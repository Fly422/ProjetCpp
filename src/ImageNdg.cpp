#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "ImageNdg.h"
#include "ImageDouble.h"

#pragma region Constructeurs & Destructeurs

CImageNdg::CImageNdg() {

	this->m_iHauteur = 0;
	this->m_iLargeur = 0;
	this->m_bBinaire = false;
	this->m_sNom = "vide";

	this->m_pucPixel = NULL;
	this->m_pucPalette = NULL;
}

CImageNdg::CImageNdg(int hauteur, int largeur, int valeur) {

	this->m_iHauteur = hauteur;
	this->m_iLargeur = largeur;
	this->m_bBinaire = false; // Image Ndg par défaut, binaire après seuillage
	this->m_sNom = "inconnu";

	this->m_pucPixel = new unsigned char[hauteur * largeur];
	this->m_pucPalette = new unsigned char[256 * 4];
	choixPalette("grise"); // palette grise par défaut, choix utilisateur 
	if (valeur != -1)
		for (int i = 0; i < this->lireNbPixels(); i++)
			this->m_pucPixel[i] = valeur;
}

CImageNdg::CImageNdg(const std::string name) {

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;

	std::ifstream f(name.c_str(), std::ios::in | std::ios::binary);
	if (f.is_open()) {
		f.read((char*)&header.bfType, 2);
		f.read((char*)&header.bfSize, 4);
		f.read((char*)&header.bfReserved1, 2);
		f.read((char*)&header.bfReserved2, 2);
		f.read((char*)&header.bfOffBits, 4);
		if (header.bfType != MAGIC_NUMBER_BMP)
			throw std::string("ouverture format BMP impossible ...");
		else {
			f.read((char*)&infoHeader.biSize, 4);
			f.read((char*)&infoHeader.biWidth, 4);
			f.read((char*)&infoHeader.biHeight, 4);
			f.read((char*)&infoHeader.biPlanes, 2);
			f.read((char*)&infoHeader.biBitCount, 2);
			f.read((char*)&infoHeader.biCompression, 4);
			f.read((char*)&infoHeader.biSizeImage, 4);
			f.read((char*)&infoHeader.biXPelsPerMeter, 4);
			f.read((char*)&infoHeader.biYPelsPerMeter, 4);
			f.read((char*)&infoHeader.biClrUsed, 4);
			f.read((char*)&infoHeader.biClrImportant, 4);
			if (infoHeader.biCompression > 0)
				throw std::string("Format compresse non supporte...");
			else {
				if (infoHeader.biBitCount == 8) {
					this->m_iHauteur = infoHeader.biHeight;
					this->m_iLargeur = infoHeader.biWidth;
					this->m_bBinaire = false;
					this->m_sNom.assign(name.begin(), name.end() - 4);
					this->m_pucPalette = new unsigned char[256 * 4];
					this->m_pucPixel = new unsigned char[infoHeader.biHeight * infoHeader.biWidth];

					// gérer multiple de 32 bits via zéros éventuels ignorés
					int complement = (((this->m_iLargeur - 1) / 4) + 1) * 4 - this->m_iLargeur;
					for (int indice = 0; indice < 4 * 256; indice++)
						f.read((char*)&this->m_pucPalette[indice], sizeof(char));

					for (int i = this->m_iHauteur - 1; i >= 0; i--) {
						for (int j = 0; j < this->m_iLargeur; j++)
							f.read((char*)&this->m_pucPixel[i * this->m_iLargeur + j], sizeof(char));

						char inutile;
						for (int k = 0; k < complement; k++)
							f.read((char*)&inutile, sizeof(char));
					}
				}
				else {
					// cas d'une image couleur
					this->m_iHauteur = infoHeader.biHeight;
					this->m_iLargeur = infoHeader.biWidth;
					this->m_bBinaire = false;
					this->m_sNom.assign(name.begin(), name.end() - 4);
					this->m_pucPalette = new unsigned char[256 * 4];
					this->choixPalette("grise"); // palette grise par défaut
					this->m_pucPixel = new unsigned char[infoHeader.biHeight * infoHeader.biWidth];

					// extraction plan luminance
					int complement = (((this->m_iLargeur * 3 - 1) / 4) + 1) * 4 - this->m_iLargeur * 3;
					for (int i = this->m_iHauteur - 1; i >= 0; i--) {
						for (int j = 0; j < this->m_iLargeur * 3; j += 3) {
							unsigned char rouge, vert, bleu;
							f.read((char*)&rouge, sizeof(char));
							f.read((char*)&vert, sizeof(char));
							f.read((char*)&bleu, sizeof(char));
							this->m_pucPixel[i * this->m_iLargeur + j / 3] = (unsigned char)(((int)rouge + (int)vert + (int)bleu) / 3);
						}

						char inutile;
						for (int k = 0; k < complement; k++)
							f.read((char*)&inutile, sizeof(char));
					}
				}
			}
		}
		f.close();
	}
	else
		throw std::string("ERREUR : Image absente (ou pas ici en tout cas) !");
}

CImageNdg::CImageNdg(const CImageNdg& im) {

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_bBinaire = im.lireBinaire();
	this->m_sNom = im.lireNom();
	this->m_pucPixel = NULL;
	this->m_pucPalette = NULL;

	if (im.m_pucPalette != NULL) {
		this->m_pucPalette = new unsigned char[256 * 4];
		memcpy(this->m_pucPalette, im.m_pucPalette, 4 * 256);
	}
	if (im.m_pucPixel != NULL) {
		this->m_pucPixel = new unsigned char[im.lireHauteur() * im.lireLargeur()];
		memcpy(this->m_pucPixel, im.m_pucPixel, im.lireNbPixels());
	}
}

CImageNdg::~CImageNdg() {
	if (this->m_pucPixel) {
		delete[] this->m_pucPixel;
		this->m_pucPixel = NULL;
	}

	if (this->m_pucPalette) {
		delete[] this->m_pucPalette;
		this->m_pucPalette = NULL;
	}
}

#pragma endregion

#pragma region Surcharge opérateur

CImageNdg& CImageNdg::operator=(const CImageNdg& im) {

	if (&im == this)
		return *this;

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_bBinaire = im.lireBinaire();
	this->m_sNom = im.lireNom();

	if (this->m_pucPixel)
		delete[] this->m_pucPixel;
	this->m_pucPixel = new unsigned char[this->m_iHauteur * this->m_iLargeur];

	if (this->m_pucPalette)
		delete[] this->m_pucPalette;
	this->m_pucPalette = new unsigned char[256 * 4];

	if (im.m_pucPalette != NULL)
		memcpy(this->m_pucPalette, im.m_pucPalette, 4 * 256);
	if (im.m_pucPixel != NULL)
		memcpy(this->m_pucPixel, im.m_pucPixel, im.lireNbPixels());

	return *this;
}

#pragma endregion

#pragma region Sauvegarde

void CImageNdg::sauvegarde(const std::string file) {
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;

	if (this->m_pucPixel) {

		std::string nomFichier = "res/";

		if (file.empty())
		{
			nomFichier += this->lireNom() + ".bmp"; // force sauvegarde dans répertoire res (doit exister)
		}
		else
		{
			nomFichier += file + ".bmp";
		}


		std::ofstream f(nomFichier.c_str(), std::ios::binary);
		if (f.is_open()) {

			int complement = (((this->m_iLargeur - 1) / 4) + 1) * 4 - this->m_iLargeur;

			header.bfType = MAGIC_NUMBER_BMP;
			f.write((char*)&header.bfType, 2);
			header.bfOffBits = 14 * sizeof(char) + 40 * sizeof(char) + 4 * 256 * sizeof(char); // palette
			header.bfSize = header.bfOffBits + (complement + lireLargeur()) * lireHauteur() * sizeof(char);
			f.write((char*)&header.bfSize, 4);
			header.bfReserved1 = 0;
			f.write((char*)&header.bfReserved1, 2);
			header.bfReserved2 = 0;
			f.write((char*)&header.bfReserved2, 2);
			f.write((char*)&header.bfOffBits, 4);

			infoHeader.biSize = 40 * sizeof(char);
			f.write((char*)&infoHeader.biSize, 4);
			infoHeader.biWidth = this->m_iLargeur;
			f.write((char*)&infoHeader.biWidth, 4);
			infoHeader.biHeight = this->m_iHauteur;
			f.write((char*)&infoHeader.biHeight, 4);
			infoHeader.biPlanes = 1;
			f.write((char*)&infoHeader.biPlanes, 2);
			infoHeader.biBitCount = 8;
			f.write((char*)&infoHeader.biBitCount, 2);
			infoHeader.biCompression = 0; // pas de compression
			f.write((char*)&infoHeader.biCompression, 4);
			infoHeader.biSizeImage = this->lireNbPixels();
			f.write((char*)&infoHeader.biSizeImage, 4);
			infoHeader.biXPelsPerMeter = 0;
			f.write((char*)&infoHeader.biXPelsPerMeter, 4);
			infoHeader.biYPelsPerMeter = 0;
			f.write((char*)&infoHeader.biYPelsPerMeter, 4);
			infoHeader.biClrUsed = 256;
			f.write((char*)&infoHeader.biClrUsed, 4);
			infoHeader.biClrImportant = 0;
			f.write((char*)&infoHeader.biClrImportant, 4);

			// on remplit la palette
			for (int indice = 0; indice < 4 * 256; indice++)
				f.write((char*)&this->m_pucPalette[indice], sizeof(char));

			for (int i = this->m_iHauteur - 1; i >= 0; i--) {
				for (int j = 0; j < m_iLargeur; j++)
					f.write((char*)&this->m_pucPixel[i * m_iLargeur + j], sizeof(char));

				// gérer multiple de 32 bits
				char inutile;
				for (int k = 0; k < complement; k++)
					f.write((char*)&inutile, sizeof(char));
			}
			f.close();
		}
		else
			throw std::string("Impossible de creer le fichier de sauvegarde !");
	}
	else
		throw std::string("Pas de donnee a sauvegarder !");
}

#pragma endregion

#pragma region Histogramme

std::vector<unsigned long> CImageNdg::histogramme(bool enregistrementCSV) {

	std::vector<unsigned long> h;

	h.resize(256, 0);
	for (int i = 0; i < this->lireNbPixels(); i++)
		h[this->operator()(i)] += 1L;

	if (enregistrementCSV) {
		std::string fichier = "res/" + this->lireNom() + ".csv";
		std::ofstream f(fichier.c_str());

		if (!f.is_open())
			throw std::string("Impossible d'ouvrir le fichier en ecriture !");
		else {
			for (int i = 0; i < (int)h.size(); i++)
				f << h[i] << std::endl;
		}
		f.close();
	}

	return h;
}

#pragma endregion

#pragma region Signatures

MOMENTS CImageNdg::signatures(const std::vector<unsigned long> h) {

	MOMENTS globales;

	// min
	int i = 0;
	while ((i < ((int)h.size()) - 1) && (h[i] == 0))
		i++;
	globales.minNdg = i;

	// max
	i = h.size() - 1;
	while ((i > 0) && (h[i] == 0))
		i--;
	globales.maxNdg = i;

	// mediane
	int moitPop = this->lireNbPixels() / 2;

	i = globales.minNdg;
	int somme = h[i];
	while (somme < moitPop) {
		i += 1;
		if (i < (int)h.size())
			somme += h[i];
	}
	globales.medianeNdg = i;

	// moyenne et écart-type
	float moy = 0, sigma = 0;
	for (i = globales.minNdg; i <= globales.maxNdg; i++) {
		moy += ((float)h[i]) * i;
		sigma += ((float)h[i]) * i * i;
	}
	moy /= (float)this->lireNbPixels();
	sigma = sqrt(sigma / (float)this->lireNbPixels() - (moy * moy));
	globales.moyenneNdg = moy;
	globales.ecartTypeNdg = sigma;

	return globales;
}

MOMENTS CImageNdg::signatures() {

	MOMENTS globales = { 0,0,0,0,0 };

	std::vector<unsigned long> hist;
	hist = this->histogramme();

	globales = this->signatures(hist);

	return globales;
}

MOMENTS_COMPOSANTES* CImageNdg::signaturesComposantes(int nbComp)
{
	MOMENTS_COMPOSANTES* SignatureCC = (MOMENTS_COMPOSANTES*)calloc((nbComp + 1), sizeof(MOMENTS_COMPOSANTES));

	//Histogramme image labelisé donne surface des composantes
	std::vector<unsigned long> histo = this->histogramme();

	//Histogramme image contour donne périmetre composantes
	std::vector<unsigned long> histoContour = this->operation(this->morphologie("erosion", "V4"), "-").histogramme();

	//SURFACE + RAYON + PERIMETRE + COMPACITE
	for (int i = 0; i < nbComp + 1; i++)
	{
		SignatureCC[i].surface = histo[i];
		SignatureCC[i].rayon = sqrt(SignatureCC[i].surface / 3.14159265);
		SignatureCC[i].perimetre = histoContour[i];
		SignatureCC[i].compacite = (SignatureCC[i].perimetre * SignatureCC[i].perimetre) / (4 * 3.14159265 * SignatureCC[i].surface);
		SignatureCC[i].suppr = 1;
	}

	//CENTRE GRAVITE
	int compo;
	for (int i = 0; i < this->lireHauteur(); i++)
	{
		for (int j = 0; j < this->lireLargeur(); j++)
		{
			compo = this->operator()(i, j);
			SignatureCC[compo].CG.abscisse = (SignatureCC[compo].CG.abscisse + j);
			SignatureCC[compo].CG.ordonnee = (SignatureCC[compo].CG.ordonnee + i);
		}
	}

	for (int compo = 0; compo < nbComp + 1; compo++)
	{
		SignatureCC[compo].CG.abscisse = (SignatureCC[compo].CG.abscisse / SignatureCC[compo].surface);
		SignatureCC[compo].CG.ordonnee = (SignatureCC[compo].CG.ordonnee / SignatureCC[compo].surface);
	}

	//BORDS
	//BORD HAUT
	for (int j = 0; j < this->lireLargeur(); j++)
	{
		compo = this->operator()(0, j);
		if (compo != 0)
		{
			SignatureCC[compo].bord = 1;
		}
	}
	//BORD BAS
	for (int j = 0; j < this->lireLargeur(); j++)
	{
		compo = this->operator()(this->lireHauteur() - 1, j);
		if (compo != 0)
		{
			SignatureCC[compo].bord = 1;
		}
	}
	//BORD GAUCHE
	for (int i = 0; i < this->lireHauteur(); i++)
	{
		compo = this->operator()(i, 0);
		if (compo != 0)
		{
			SignatureCC[compo].bord = 1;
		}
	}
	//BORD DROIT
	for (int i = 0; i < this->lireHauteur(); i++)
	{
		compo = this->operator()(i, this->lireLargeur() - 1);
		if (compo != 0)
		{
			SignatureCC[compo].bord = 1;
		}
	}

	return SignatureCC;
}

#pragma endregion

#pragma region Scores

double CImageNdg::score(const CImageNdg& im, const std::string methode)
{
	double res;
	if (this->m_bBinaire && im.m_bBinaire)
	{
		if (methode == "iou")
		{
			CImageNdg inter = this->operation(im, "et");
			CImageNdg unio = this->operation(im, "ou");

			std::vector<unsigned long> h_inter = inter.histogramme();
			std::vector<unsigned long> h_union = unio.histogramme();

			res = ((float)h_inter[1] / (float)h_union[1]) * 100;
		}
	}

	return res;
}

#pragma endregion

#pragma region Traitements

CImageNdg CImageNdg::operation(const CImageNdg& im, const std::string methode) {

	if ((&im == this) || !(this->lireBinaire() && im.lireBinaire())) {
		throw std::string("Operation logique uniquement possible entre 2 images binaires");
		return *this;
	}

	CImageNdg out(this->lireHauteur(), this->lireLargeur());
	out.m_bBinaire = im.lireBinaire();
	out.choixPalette("binaire"); // images binaires -> palettes binaires
	out.m_sNom = im.lireNom() + "Op";

	if (methode.compare("et") == 0) {
		for (int i = 0; i < this->lireNbPixels(); i++)
		{
			out.operator()(i) = this->operator()(i) && im(i);
		}
	}
	else
		if (methode.compare("ou") == 0) {
			for (int i = 0; i < this->lireNbPixels(); i++)
				out.operator()(i) = this->operator()(i) || im(i);
		}
		else
			if (methode.compare("-") == 0) {
				for (int i = 0; i < this->lireNbPixels(); i++)
					out.operator()(i) = this->operator()(i) - im(i); // vigilance sur les images opérérées !
			}

	return out;
}

CImageNdg CImageNdg::seuillage(const std::string methode, int seuilBas, int seuilHaut) {

	if (!this->m_bBinaire) {
		CImageNdg out(this->lireHauteur(), this->lireLargeur());
		out.choixPalette("grise"); // palette binaire par défaut
		out.m_bBinaire = true;

		// création lut pour optimisation calcul
		std::vector<int> lut;
		lut.resize(256);

		// recherche valeur seuil
		// cas "manuel" 
		if (methode.compare("manuel") == 0)
		{
			out.m_sNom = this->lireNom() + "SeMa";
		}
		else
			if (methode.compare("otsu") == 0)
			{
				out.m_sNom = this->lireNom() + "SeAu";
				// recherche seuil via Otsu
				std::vector<unsigned long> hist = this->histogramme();
				std::vector<unsigned long> histC; // histogramme cumulé
				histC.resize(256, 0);
				histC[0] = hist[0];
				for (int i = 1; i < (int)hist.size(); i++)
					histC[i] = histC[i - 1] + hist[i];

				MOMENTS globales = this->signatures(hist);
				int min = globales.minNdg,
					max = globales.maxNdg;

				// f(s)
				std::vector<double> tab;
				tab.resize(256, 0);

				double M1, M2, w1;

				// initialisation
				M1 = min;
				seuilBas = min;
				seuilHaut = 255;

				w1 = (double)histC[min] / (double)(this->lireNbPixels());
				M2 = 0;
				for (int i = min + 1; i <= max; i++)
					M2 += (double)hist[i] * i;
				M2 /= (double)(histC[max] - hist[min]);
				tab[min] = w1 * (1 - w1) * (M1 - M2) * (M1 - M2);

				for (int i = min + 1; i < max; i++) {
					M1 = ((double)histC[i - 1] * M1 + (double)hist[i] * i) / histC[i];
					M2 = ((double)(histC[255] - histC[i - 1]) * M2 - hist[i] * i) / (double)(histC[255] - histC[i]);
					w1 = (double)histC[i] / (double)(this->lireNbPixels());
					tab[i] = w1 * (1 - w1) * (M1 - M2) * (M1 - M2);
					if (tab[i] > tab[seuilBas])
						seuilBas = i;
				}
			}
			else
			{
				if (methode.compare("moyenne") == 0)
				{
					out.m_sNom = this->lireNom() + "SeMoy";

					seuilBas = (int)this->signatures().moyenneNdg;
					seuilHaut = 255;
				}
				else
				{
					if (methode.compare("mediane") == 0)
					{
						out.m_sNom = this->lireNom() + "SeMed";
						seuilBas = this->signatures().medianeNdg;
						seuilHaut = 255;
					}
				}
			}



		// génération lut
		for (int i = 0; i < seuilBas; i++)
			lut[i] = 0;
		for (int i = seuilBas; i <= seuilHaut; i++)
			lut[i] = 255;
		for (int i = seuilHaut + 1; i <= 255; i++)
			lut[i] = 0;

		// création image seuillée
		std::cout << "Seuillage des pixels entre " << seuilBas << " et " << seuilHaut << std::endl;
		for (int i = 0; i < out.lireNbPixels(); i++)
			out(i) = lut[this->operator ()(i)];

		return out;
	}
	else {
		throw std::string("Seuillage image binaire impossible");
		return (*this);
	}
}

CImageNdg CImageNdg::transformation(const std::string methode) {

	CImageNdg out(this->lireHauteur(), this->lireLargeur());

	std::vector<int> map = this->lirePalette();
	out.choixPalette(map); // conservation de la palette
	out.m_bBinaire = this->m_bBinaire; // conservation du type

	if (methode.compare("complement") == 0) {
		out.m_sNom = this->lireNom() + "TComp";
		// ndg -> 255-ndg
		// création lut pour optimisation calcul
		std::vector<int> lut;
		lut.resize(256);

		if (!this->lireBinaire())
		{
			for (int i = 0; i < 256; i++)
				lut[i] = (int)(255 - i);
		}
		else
		{
			for (int i = 0; i < 2; i++)
				lut[i] = (int)(255 - i);
		}

		for (int i = 0; i < out.lireNbPixels(); i++)
			out(i) = lut[this->operator()(i)];
	}
	else
		if (methode.compare("expansion") == 0) {
			out.m_sNom = this->lireNom() + "TExp";
			int min = 255, max = 0;
			for (int i = 0; i < this->lireNbPixels(); i++) {
				if (this->operator()(i) > max)
					max = this->operator()(i);
				if (this->operator()(i) < min)
					min = this->operator()(i);
			}

			double a = 255 / (double)(max - min);
			double b = -a * min;

			std::vector<int> lut;
			lut.resize(256);

			for (int i = 0; i < 256; i++)
				lut[i] = (int)(a * i + b);
			for (int i = 0; i < out.lireNbPixels(); i++)
				out(i) = lut[this->operator()(i)];
		}
		else
			if (methode.compare("egalisation") == 0) {
				out.m_sNom = this->lireNom() + "TEga";
				std::vector<unsigned long> h = this->histogramme();
				std::vector<unsigned long> hC = h;
				for (int i = 1; i < (int)h.size(); i++)
					hC[i] = hC[i - 1] + h[i];

				// recherche min et max image
				int min = 0, max = 255;
				int i = 0;
				while (h[i] == 0)
					i++;
				min = i;
				i = 255;
				while (h[i] == 0)
					i--;
				max = i;

				std::vector<int> lut;
				lut.resize(256);

				for (int i = min; i <= max; i++)
					lut[i] = (int)(((double)hC[i] / (double)this->lireNbPixels()) * (double)255);

				for (int i = 0; i < out.lireNbPixels(); i++)
					out(i) = lut[this->operator()(i)];
			}

	return out;
}

CImageNdg CImageNdg::morphologie(const std::string methode, const std::string eltStructurant, const int nbIteration) {

	CImageNdg out(this->lireHauteur(), this->lireLargeur());

	out.choixPalette(this->lirePalette()); // conservation de la palette

	out.m_bBinaire = this->m_bBinaire; // conservation du type

	if (methode.compare("erosion") == 0) {
		out.m_sNom = this->lireNom() + "MEr";
		CImageNdg agrandie(this->lireHauteur() + 2, this->lireLargeur() + 2);

		// gestion des bords
		if (this->lireBinaire()) {
			int pix;

			for (pix = 0; pix < agrandie.lireLargeur(); pix++) {
				agrandie(0, pix) = 1;
				agrandie(this->lireHauteur() - 1, pix) = 1;
			}
			for (pix = 1; pix < agrandie.lireHauteur() - 1; pix++) {
				agrandie(pix, 0) = 1;
				agrandie(pix, this->lireLargeur() - 1) = 1;
			}
		}
		else {
			int pix;

			for (pix = 0; pix < agrandie.lireLargeur(); pix++) {
				agrandie(0, pix) = 255;
				agrandie(this->lireHauteur() - 1, pix) = 255;
			}
			for (pix = 1; pix < agrandie.lireHauteur() - 1; pix++) {
				agrandie(pix, 0) = 255;
				agrandie(pix, this->lireLargeur() - 1) = 255;
			}
		}

		// gestion du coeur
		for (int i = 0; i < this->lireHauteur(); i++)
			for (int j = 0; j < this->lireLargeur(); j++) {
				agrandie(i + 1, j + 1) = this->operator()(i, j);
			}

		if (eltStructurant.compare("V4") == 0) {
			for (int i = 1; i < agrandie.lireHauteur() - 1; i++)
				for (int j = 1; j < agrandie.lireLargeur() - 1; j++) {
					int minH = std::min(agrandie(i, j - 1), agrandie(i, j + 1));
					int minV = std::min(agrandie(i - 1, j), agrandie(i + 1, j));
					int minV4 = std::min(minH, minV);
					out(i - 1, j - 1) = std::min(minV4, (int)agrandie(i, j));
				}
		}
		else {
			if (eltStructurant.compare("V8") == 0) {
				for (int i = 1; i < agrandie.lireHauteur() - 1; i++)
					for (int j = 1; j < agrandie.lireLargeur() - 1; j++) {
						int minH = std::min(agrandie(i, j - 1), agrandie(i, j + 1));
						int minV = std::min(agrandie(i - 1, j), agrandie(i + 1, j));
						int minV4 = std::min(minH, minV);
						int minD1 = std::min(agrandie(i - 1, j - 1), agrandie(i + 1, j + 1));
						int minD2 = std::min(agrandie(i - 1, j + 1), agrandie(i + 1, j - 1));
						int minD = std::min(minD1, minD2);
						int minV8 = std::min(minV4, minD);
						out(i - 1, j - 1) = std::min(minV8, (int)agrandie(i, j));
					}
			}
		}
	}
	else {
		if (methode.compare("dilatation") == 0) {
			out.m_sNom = this->lireNom() + "MDi";
			CImageNdg agrandie(this->lireHauteur() + 2, this->lireLargeur() + 2);

			// gestion des bords
			int pix;

			for (pix = 0; pix < agrandie.lireLargeur(); pix++) {
				agrandie(0, pix) = 0;
				agrandie(agrandie.lireHauteur() - 1, pix) = 0;
			}
			for (pix = 1; pix < agrandie.lireHauteur() - 1; pix++) {
				agrandie(pix, 0) = 0;
				agrandie(pix, agrandie.lireLargeur() - 1) = 0;
			}

			// gestion du coeur
			for (int i = 0; i < this->lireHauteur(); i++)
				for (int j = 0; j < this->lireLargeur(); j++) {
					agrandie(i + 1, j + 1) = this->operator()(i, j);
				}

			if (eltStructurant.compare("V4") == 0) {
				for (int i = 1; i < agrandie.lireHauteur() - 1; i++)
					for (int j = 1; j < agrandie.lireLargeur() - 1; j++) {
						int maxH = std::max(agrandie(i, j - 1), agrandie(i, j + 1));
						int maxV = std::max(agrandie(i - 1, j), agrandie(i + 1, j));
						int maxV4 = std::max(maxH, maxV);
						out(i - 1, j - 1) = std::max(maxV4, (int)agrandie(i, j));
					}
			}
			else {
				if (eltStructurant.compare("V8") == 0) {
					for (int i = 1; i < agrandie.lireHauteur() - 1; i++)
						for (int j = 1; j < agrandie.lireLargeur() - 1; j++) {
							int maxH = std::max(agrandie(i, j - 1), agrandie(i, j + 1));
							int maxV = std::max(agrandie(i - 1, j), agrandie(i + 1, j));
							int maxV4 = std::max(maxH, maxV);
							int maxD1 = std::max(agrandie(i - 1, j - 1), agrandie(i + 1, j + 1));
							int maxD2 = std::max(agrandie(i - 1, j + 1), agrandie(i + 1, j - 1));
							int maxD = std::max(maxD1, maxD2);
							int maxV8 = std::max(maxV4, maxD);
							out(i - 1, j - 1) = std::max(maxV8, (int)agrandie(i, j));
						}
				}
			}
		}
	}
	if (methode.compare("ouverture") == 0)
	{
		if (eltStructurant.compare("V4") == 0)
		{
			out = this->morphologie("erosion", "V4");
			out = out.morphologie("dilatation", "V4");
		}

		if (eltStructurant.compare("V8") == 0)
		{
			out = this->morphologie("erosion", "V8");
			out = out.morphologie("dilatation", "V8");
		}
	}
	if (methode.compare("fermeture") == 0)
	{
		if (eltStructurant.compare("V4") == 0)
		{
			out = this->morphologie("dilatation", "V4");
			out = out.morphologie("erosion", "V4");
		}

		if (eltStructurant.compare("V8") == 0)
		{
			out = this->morphologie("dilatation", "V8");
			out = out.morphologie("erosion", "V8");
		}
	}
	if (methode.compare("whiteTopHat") == 0)
	{
		CImageNdg wth(out);

		if (eltStructurant.compare("V4") == 0)
		{
			wth = this->morphologie("erosion", "V4");
			for (int i = 0; i < nbIteration - 1; i++)
			{
				wth = wth.morphologie("erosion", "V4");
			}
			for (int i = 0; i < nbIteration; i++)
			{
				wth = wth.morphologie("dilatation", "V4");
			}
		}

		if (eltStructurant.compare("V8") == 0)
		{
			wth = this->morphologie("erosion", "V8");
			for (int i = 0; i < nbIteration - 1; i++)
			{
				wth = wth.morphologie("erosion", "V8");
			}
			for (int i = 0; i < nbIteration; i++)
			{
				wth = wth.morphologie("dilatation", "V8");
			}
		}

		int temp;
		for (int i = 0; i < this->lireNbPixels(); i++)
		{
			temp = this->operator()(i) - wth(i);

			//if (temp < 0)
			//{
			//	temp = 0;
			//}
			//if (temp > 255)
			//{
			//	temp = 255;
			//}

			out(i) = temp;
		}
	}
	if (methode.compare("blackTopHat") == 0)
	{
		CImageNdg bth(out);

		if (eltStructurant.compare("V4") == 0)
		{
			bth = this->morphologie("dilatation", "V4");
			for (int i = 0; i < nbIteration - 1; i++)
			{
				bth = bth.morphologie("dilatation", "V4");
			}
			for (int i = 0; i < nbIteration; i++)
			{
				bth = bth.morphologie("erosion", "V4");
			}
		}

		if (eltStructurant.compare("V8") == 0)
		{
			bth = this->morphologie("dilatation", "V8");
			for (int i = 0; i < nbIteration - 1; i++)
			{
				bth = bth.morphologie("dilatation", "V8");
			}
			for (int i = 0; i < nbIteration; i++)
			{
				bth = bth.morphologie("erosion", "V8");
			}
		}

		int temp;
		for (int i = 0; i < this->lireNbPixels(); i++)
		{
			temp = this->operator()(i) - bth(i);

			//if (temp < 0)
			//{
			//	temp = 0;
			//}
			//if (temp > 255)
			//{
			//	temp = 255;
			//}

			out(i) = temp;
		}
	}

	return out;
}

CImageNdg CImageNdg::ecartTypeLocal(const std::string methode, const int N)
{
	CImageNdg out(this->lireHauteur(), this->lireLargeur());
	out.m_bBinaire = this->m_bBinaire;
	out.m_pucPalette = this->m_pucPalette;

	if (methode.compare("ecart type") == 0)
	{
		out.m_sNom = this->m_sNom + "ETL";

		double moy = 0;
		double ecartType = 0;
		double somme = 0;

		int nbPixel = 0;
		int N = 3;

		for (int i = 0; i < this->lireHauteur(); i++)
		{
			for (int j = 0; j < this->lireLargeur(); j++)
			{
				moy = 0;
				ecartType = 0;
				somme = 0;
				nbPixel = 0;

				for (int k = -N / 2; k <= N / 2; k++)
				{
					for (int l = -N / 2; l <= N / 2; l++)
					{
						if (i + k >= 0 && i + k < this->lireHauteur() && j + l >= 0 && j + l < this->lireLargeur())
						{
							moy += (double)this->operator()(i + k, j + l);
							nbPixel++;
						}
					}
				}

				moy /= nbPixel;

				for (int k = -N / 2; k <= N / 2; k++)
				{
					for (int l = -N / 2; l <= N / 2; l++)
					{
						if (i + k >= 0 && i + k < this->lireHauteur() && j + l >= 0 && j + l < this->lireLargeur())
						{
							somme += ((double)this->operator()(i + k, j + l) - moy) * ((double)this->operator()(i + k, j + l) - moy);
						}
					}
				}

				ecartType = sqrt(somme / nbPixel);

				out(i, j) = ecartType;

			}
		}
	}

	return out;
}

CImageNdg CImageNdg::labelImage(int* nbComp)
{
	CImageNdg out(this->lireHauteur(), this->lireLargeur());
	out.choixPalette("binaire");
	out.m_bBinaire = this->m_bBinaire;

	int i, ii, j, jj;

	int k = 1, kk;
	int eps;
	int predNonNul[5];
	int nbPredNonNuls;

	int nbMaxEtiquettes = this->lireHauteur() * this->lireLargeur();
	CImageNdg withBords(this->lireHauteur() + 2, this->lireLargeur() + 2, 0);

	for (i = 0, ii = 1; i < this->lireHauteur(); i++, ii++) // pure copie 
		for (j = 0, jj = 1; j < this->lireLargeur(); j++, jj++)
			withBords(ii, jj) = (int)this->operator()(i, j);

	int* tableEtiquette = NULL;
	tableEtiquette = (int*)calloc(nbMaxEtiquettes, sizeof(int));

	for (kk = 0; kk < nbMaxEtiquettes; kk++) {
		tableEtiquette[kk] = kk;
	}

	for (i = 1; i < withBords.lireHauteur() - 1; i++)
		for (j = 1; j < withBords.lireLargeur() - 1; j++) {
			if (withBords(i, j) != 0) {
				// voisinage V8 
				int pred1 = withBords(i, j - 1);
				int pred2 = withBords(i - 1, j - 1);
				int pred3 = withBords(i - 1, j);
				int pred4 = withBords(i - 1, j + 1);

				nbPredNonNuls = 0;
				if (pred1) predNonNul[nbPredNonNuls++] = pred1;
				if (pred2) predNonNul[nbPredNonNuls++] = pred2;
				if (pred3) predNonNul[nbPredNonNuls++] = pred3;
				if (pred4) predNonNul[nbPredNonNuls++] = pred4;

				if (nbPredNonNuls == 0) {
					withBords(i, j) = k;
					k++;
				}
				else {
					eps = 0;
					int minPred = tableEtiquette[predNonNul[0]];
					int maxPred = tableEtiquette[predNonNul[0]];
					while (eps < nbPredNonNuls) {
						if (tableEtiquette[predNonNul[eps]] < minPred) minPred = tableEtiquette[predNonNul[eps]];
						if (tableEtiquette[predNonNul[eps]] > maxPred) maxPred = tableEtiquette[predNonNul[eps]];
						eps++;
					}
					if (minPred == maxPred)
						withBords(i, j) = minPred;
					else {
						withBords(i, j) = minPred;
						for (eps = 0; eps < nbPredNonNuls; eps++) {
							int beta = predNonNul[eps];
							while (tableEtiquette[beta] != minPred) {
								int m = tableEtiquette[beta];
								tableEtiquette[beta] = minPred;
								beta = m;
							}
						}
					}
				}
			}
		}


	// actualisation de la table d'équivalence
	for (int kk = 1; kk < k; kk++) {
		int m = kk;
		while (tableEtiquette[m] != m)
			m = tableEtiquette[m];
		tableEtiquette[kk] = m;
	}

	int* etiquettes = (int*)calloc(k + 1, sizeof(int));

	// histo pour repérer les trous
	for (int kk = 1; kk < k; kk++)
		etiquettes[tableEtiquette[kk]]++;

	// on remet à jour les index (etiquetage définitif hors trou)
	etiquettes[0] = 0;
	int compt = 1;
	for (int kk = 1; kk < k; kk++) {
		if (etiquettes[kk])
			etiquettes[kk] = compt++;
	}

	for (i = 0; i < withBords.lireHauteur(); i++)
		for (j = 0; j < withBords.lireLargeur(); j++) {
			withBords(i, j) = etiquettes[tableEtiquette[withBords(i, j)]];
		}

	free(etiquettes);
	free(tableEtiquette);

	*nbComp = compt - 1;
	for (i = 0, ii = 1; i < this->lireHauteur(); i++, ii++)
		for (j = 0, jj = 1; j < this->lireLargeur(); j++, jj++)
			out(i, j) = withBords(ii, jj);

	return out;
}

CImageNdg CImageNdg::hough(int threshold, bool diffLine)
{
	CImageDouble imgDouble(*this);
	CImageNdg imgContours = imgDouble.vecteurGradient("norme").toNdg();
	imgContours.sauvegarde("Contours");
	CImageNdg res(this->lireHauteur(), this->lireLargeur(), 0);

	int height = this->lireHauteur();
	int width = this->lireLargeur();

	const int maxTheta = 180;  // Nombre d'angles possibles (degrés)
	const int maxRho = static_cast<int>(sqrt(width * width + height * height));  // Distance maximale possible depuis l'origine

	std::vector<std::vector<int>> accumulator(maxTheta, std::vector<int>(maxRho, 0));

	// Appliquer la transformée de Hough
	for (int x = 0; x < height; ++x)
	{
		for (int y = 0; y < width; ++y)
		{
			if (imgContours(x, y) != 0)
			{  // Si le pixel est un contour
				for (int theta = 0; theta < maxTheta; theta++)
				{
					double radians = theta * 3.14159274101257324219 / 180.0;
					int rho = static_cast<int>(y * cos(radians) + x * sin(radians));

					if (rho >= 0 && rho < maxRho)
					{
						accumulator[theta][rho]++;
					}
				}
			}
		}
	}

	CImageNdg hough(accumulator.size(), accumulator[0].size());

	for (int i = 0; i < hough.lireHauteur(); i++)
	{
		for (int j = 0; j < hough.lireLargeur(); j++)
		{
			hough(i, j) = accumulator[i][j];
		}
	}

	hough.sauvegarde("Sinus");

	// Trouver les points maximums dans l'accumulateur
	std::vector<std::pair<int, int>> houghLines;
	for (int theta = 0; theta < maxTheta; theta++)
	{
		for (int rho = 0; rho < maxRho; rho++)
		{
			// Seuil arbitraire pour la détection
			if (accumulator[theta][rho] >= threshold)
			{
				bool isLocalMaxima = true;

				// Check les voisins
				for (int dTheta = -1; dTheta <= 1; ++dTheta)
				{
					for (int dRho = -1; dRho <= 1; ++dRho)
					{
						int neighborTheta = theta + dTheta;
						int neighborRho = rho + dRho;

						if (neighborTheta < 0 || neighborTheta >= maxTheta || neighborRho < 0 || neighborRho >= maxRho)
						{
							continue;
						}

						// Check si le voisin est un maxima 
						if (accumulator[neighborTheta][neighborRho] > accumulator[theta][rho])
						{
							isLocalMaxima = false;
							break;
						}
					}
					if (!isLocalMaxima)
					{
						break;
					}
				}

				// Si ligne est un maxima alors on garde
				if (isLocalMaxima)
				{
					houghLines.push_back({ theta, rho });
				}
			}
		}
	}

	if (diffLine)
	{
		int grayIncrement = 255 / houghLines.size();
		int currentGrayLevel = 1;

		for (const auto& line : houghLines)
		{
			int theta = line.first;
			int rho = line.second;

			// Convertir les coordonnées polaires en coordonnées cartésiennes
			double radians = theta * 3.14159274101257324219 / 180.0;

			for (int x = 0; x < width; x++)
			{
				int y = static_cast<int>((rho - x * cos(radians)) / sin(radians));
				if (y >= 0 && y < height)
				{
					// Dessiner un pixel sur l'image résultante
					res(y, x) = currentGrayLevel;
				}
			}

			for (int y = 0; y < height; y++)
			{
				int x = static_cast<int>((rho - y * sin(radians)) / cos(radians));
				if (x >= 0 && x < width)
				{
					// Dessiner un pixel sur l'image résultante
					res(y, x) = currentGrayLevel;
				}
			}

			currentGrayLevel += grayIncrement;
			grayIncrement++; //Incrémentation pour éviter de reboucler sur une même couleur
			if (currentGrayLevel >= 255)
			{
				currentGrayLevel = 0;  // Réinitialiser si on dépasse la valeur maximale
			}
		}

		return res;
	}
	else
	{
		for (const auto& line : houghLines)
		{
			int theta = line.first;
			int rho = line.second;

			// Convertir les coordonnées polaires en coordonnées cartésiennes
			double radians = theta * 3.14159274101257324219 / 180.0;

			for (int x = 0; x < width; x++)
			{
				int y = static_cast<int>((rho - x * cos(radians)) / sin(radians));
				if (y >= 0 && y < height)
				{
					// Dessiner un pixel sur l'image résultante
					res(y, x) = 255;
				}
			}

			for (int y = 0; y < height; y++)
			{
				int x = static_cast<int>((rho - y * sin(radians)) / cos(radians));
				if (x >= 0 && x < width)
				{
					// Dessiner un pixel sur l'image résultante
					res(y, x) = 255;
				}
			}


		}

		return res;
	}
}





#pragma endregion
