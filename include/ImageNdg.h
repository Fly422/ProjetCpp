#pragma once

#ifndef _IMAGE_NDG_
#define _IMAGE_NDG_

#include <vector>
#include <string>
#include <algorithm>

#pragma region Gestion format BMP indépendamment de <windows.h>

#define MAGIC_NUMBER_BMP ('B'+('M'<<8)) 

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long LONG;

typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

#pragma endregion

#pragma region Structures MOMENTS pour signatures

typedef struct {
	int		minNdg;
	int		maxNdg;
	int		medianeNdg;
	float	moyenneNdg;
	float	ecartTypeNdg;
} MOMENTS;

#pragma endregion

#pragma region Structures MOMENTS_COMPOSANTES pour signatures des composantes connexes
typedef struct {
	float abscisse;
	float ordonnee;

} POINT;

typedef struct {
	int surface;
	float perimetre;
	float compacite;
	int bord;
	POINT CG;
	float rayon;

	int suppr;
} MOMENTS_COMPOSANTES;

#pragma endregion


class CImageNdg 
{
	private : 
		int              m_iHauteur;
		int              m_iLargeur;  
		bool			 m_bBinaire;	
		std::string      m_sNom;
		unsigned char*   m_pucPixel;
		unsigned char*   m_pucPalette; 

	public : 

#pragma region Constructeurs & Destructeurs
		// par défaut
		_declspec(dllexport) CImageNdg(); 

		// si -1 alors non pixels non initialisés
		_declspec(dllexport) CImageNdg(int hauteur, int largeur, int val=-1); 

		// fichier en entrée
		_declspec(dllexport) CImageNdg(const std::string nom);

		// image en entrée
		_declspec(dllexport) CImageNdg(const CImageNdg& im); 

		// destructeur
		_declspec(dllexport) ~CImageNdg(); 

#pragma endregion

#pragma region Get/Set

		_declspec(dllexport) int lireHauteur() const {
			return m_iHauteur;
		}

		_declspec(dllexport) int lireLargeur() const {
			return m_iLargeur;
		}

		_declspec(dllexport) bool lireBinaire() const {
			return m_bBinaire;
		}

		_declspec(dllexport) std::string lireNom() const {
			return m_sNom;
		}

		_declspec(dllexport) void ecrireHauteur(int hauteur) {
			m_iHauteur = hauteur;
		}

		_declspec(dllexport) void ecrireLargeur(int largeur) {
			m_iLargeur = largeur;
		}

		_declspec(dllexport) void ecrireBinaire(bool type) {
			m_bBinaire = type;
		}

		_declspec(dllexport) void ecrireNom(std::string nom) {
			m_sNom = nom;
		}

		_declspec(dllexport) int lireNbPixels() const {
			return m_iHauteur * m_iLargeur;
		}

		_declspec(dllexport) std::vector<int> lirePalette() const { // palette image sortie au format tableau de 256 couleurs RGB
			// palette image sortie au format tableau de 256 couleurs RGB
			std::vector<int> palette;
			palette.resize(256 * 3);
			for (int indice = 0; indice < 256; indice++) {
				palette[indice * 3 + 2] = this->m_pucPalette[indice * 4];
				palette[indice * 3 + 1] = this->m_pucPalette[indice * 4 + 1];
				palette[indice * 3] = this->m_pucPalette[indice * 4 + 2];
			}
			return palette;
		}

#pragma endregion
		
#pragma region Surcharge opérateur

		/// <summary>
		/// pouvoir accéder à un pixel par image(i)
		/// </summary>
		/// <param name="i"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) unsigned char& operator() (int i) const { 
		return m_pucPixel[i];
		}
		
		/// <summary>
		/// pouvoir accéder à un pixel par image(i,j)
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) unsigned char& operator() (int i, int j) const { 
		return m_pucPixel[i*m_iLargeur+j];
		}

		
		/// <summary>
		/// opérateur copie image par imOut = imIn
		/// </summary>
		/// <param name="im">image a copir</param>
		/// <returns>image copié</returns>
		_declspec(dllexport) CImageNdg& operator=(const CImageNdg& im);
#pragma endregion

#pragma region Sauvegarde
		/// <summary>
		/// Sauvegarde dans res
		/// </summary>
		/// <param name="file">Nom de l'image sauvegardé</param>
		_declspec(dllexport) void sauvegarde(const std::string file = "");
#pragma endregion

#pragma region Palette
		/// <summary>
		/// choisir palette
		/// </summary>
		/// <param name="name"> "rouge", "verte", "bleue", "binaire", "grise" par défaut</param>
		/// <returns>image d'entrée modifié</returns>
		_declspec(dllexport) CImageNdg& choixPalette(std::string name = "grise") {
			if (name.compare("rouge") == 0) {
				for (int indice=0;indice<256;indice ++) {
					this->m_pucPalette[indice*4]   = 0;
					this->m_pucPalette[indice*4+1] = 0;
					this->m_pucPalette[indice*4+2] = (unsigned char)(indice);
					this->m_pucPalette[indice*4+3] = 0;
				}
			}
			else
				if (name.compare("verte") == 0) {
					for (int indice=0;indice<256;indice ++) {
						this->m_pucPalette[indice*4]   = 0;
						this->m_pucPalette[indice*4+1] = (unsigned char)(indice);
						this->m_pucPalette[indice*4+2] = 0;
						this->m_pucPalette[indice*4+3] = 0;
					}
				}
				else
					if (name.compare("bleue") == 0) {
						for (int indice=0;indice<256;indice ++) {
							this->m_pucPalette[indice*4]   = (unsigned char)(indice);
							this->m_pucPalette[indice*4+1] = 0;
							this->m_pucPalette[indice*4+2] = 0;
							this->m_pucPalette[indice*4+3] = 0;
						}
					}
					else
						if (name.compare("binaire") == 0) {
							// fond toujours à 0
							this->m_pucPalette[0] = 0; this->m_pucPalette[1] = 0; this->m_pucPalette[2] = 0; this->m_pucPalette[3] = 0;

							// 8 couleurs de base
							// ensuite périodicité, de sorte que l'objet 1 a le même label que l'objet 9 par exemple
							this->m_pucPalette[4]  = 255; this->m_pucPalette[5] = 0;    this->m_pucPalette[6] = 0;    this->m_pucPalette[7] = 0;
							this->m_pucPalette[8]  = 0;   this->m_pucPalette[9] = 255;  this->m_pucPalette[10] = 0;   this->m_pucPalette[11] = 0;
							this->m_pucPalette[12] = 0;   this->m_pucPalette[13] = 0;   this->m_pucPalette[14] = 255; this->m_pucPalette[15] = 0;
							this->m_pucPalette[16] = 255; this->m_pucPalette[17] = 255; this->m_pucPalette[18] = 0;   this->m_pucPalette[19] = 0;
							this->m_pucPalette[20] = 0;   this->m_pucPalette[21] = 255; this->m_pucPalette[22] = 255; this->m_pucPalette[23] = 0;
							this->m_pucPalette[24] = 255; this->m_pucPalette[25] = 0;   this->m_pucPalette[26] = 255; this->m_pucPalette[27] = 0;
							this->m_pucPalette[28] = 0;   this->m_pucPalette[29] = 128; this->m_pucPalette[30] = 255; this->m_pucPalette[31] = 0;
							this->m_pucPalette[32] = 128; this->m_pucPalette[33] = 255;   this->m_pucPalette[34] = 128; this->m_pucPalette[35] = 0;

							for (int indice=9;indice<256;indice ++) {
								this->m_pucPalette[indice*4]   = this->m_pucPalette[indice*4-32];
								this->m_pucPalette[indice*4+1] = this->m_pucPalette[indice*4+1-32];;
								this->m_pucPalette[indice*4+2] = this->m_pucPalette[indice*4+2-32];;
								this->m_pucPalette[indice*4+3] = 0;
							}
						}
						else // palette gris
							for (int indice=0;indice<256;indice ++) {
								this->m_pucPalette[indice*4]   = (unsigned char)(indice);
								this->m_pucPalette[indice*4+1] = (unsigned char)(indice);
								this->m_pucPalette[indice*4+2] = (unsigned char)(indice);
								this->m_pucPalette[indice*4+3] = 0;
							}
			return *this;
		}
		
		/// <summary>
		/// choisir palette
		/// </summary>
		/// <param name="map">palette sous forme de vector</param>
		/// <returns>image d'entrée modifié</returns>
		_declspec(dllexport) CImageNdg& choixPalette(std::vector<int> map) { // couleurs données par tableau de 256 couleurs RGB)
			for (int indice=0;indice<256;indice ++) {
				this->m_pucPalette[indice*4]   = (unsigned char)map[indice*3+2];
				this->m_pucPalette[indice*4+1] = (unsigned char)map[indice*3+1];
				this->m_pucPalette[indice*4+2] = (unsigned char)map[indice*3];
				this->m_pucPalette[indice*4+3] = 0;
			}
			return *this;
		}
#pragma endregion
		
#pragma region Histogramme
		/// <summary>
		/// histogramme
		/// </summary>
		/// <param name="enregistrementCSV"> true pour avoir un CSV ou false sinon </param>
		/// <returns> Histogramme de l'image </returns>
		_declspec(dllexport) std::vector<unsigned long> histogramme(bool enregistrementCSV = false); 
#pragma endregion

#pragma region Signatures
		/// <summary>
		/// signatures avec histogramme préalablement calculé
		/// </summary>
		/// <param name="h">Histogramme de l'image</param>
		/// <returns>Les signatures de l'images</returns>
		_declspec(dllexport) MOMENTS signatures(const std::vector<unsigned long> h);
		
		/// <summary>
		/// signatures sans histo préalablement calculé
		/// </summary>
		/// <returns>Les signatures de l'images</returns>
		_declspec(dllexport) MOMENTS signatures();
		
		/// <summary>
		///  signatures pour chaque composante connexe => Image labélisé et [0] = fond
		/// </summary>
		/// <param name="nbComp">Nombre de composante</param>
		/// <returns></returns>
		_declspec(dllexport) MOMENTS_COMPOSANTES* signaturesComposantes(int nbComp);

#pragma endregion

#pragma region Scores
		/// <summary>
		/// score comparaison image binaire SEG/vérité terrain => res = img1 operation img2;
		/// </summary>
		/// <param name="im"> image binaire avec qui comparé</param>
		/// <param name="methode">iou</param>
		/// <returns>Un score en %</returns>
		_declspec(dllexport) double score(const CImageNdg& im, const std::string methode = "iou");

#pragma endregion

#pragma region Traitements
		/// <summary>
		/// opérations ensemblistes pour images binaires => img = img1 operation img2;
		/// </summary>
		/// <param name="im"> image binaire avec qui opéré </param>
		/// <param name="methode"> "et", "ou", "-" </param>
		/// <returns></returns>
		_declspec(dllexport) CImageNdg operation(const CImageNdg& im, const std::string methode = "et");

		/// <summary>
		/// seuillage
		/// </summary>
		/// <param name="methode"> "manuel", "otsu" par défaut </param>
		/// <param name="seuilBas"> seuilBas </param>
		/// <param name="seuilHaut"> seuilHaut </param>
		/// <returns>Image seuillé</returns>
		_declspec(dllexport) CImageNdg seuillage(const std::string methode = "otsu", int seuilBas = 128, int seuilHaut = 255); // seuillage automatique Otsu par défaut, sinon "manuel" et choix des seuils Bas et Haut

		/// <summary>
		/// transformation
		/// </summary>
		/// <param name="methode"> "complement" par defaut, "expansion", "egalisation" </param>
		/// <returns> Image issue de la transfo </returns>
		_declspec(dllexport) CImageNdg transformation(const std::string methode = "complement"); 

		/// <summary>
		/// morphologie
		/// </summary>
		/// <param name="methode"> "erosion", "dilatation" par defaut, "ouverture", "fermeture", "whiteTopHat", "blackTopHat" </param>
		/// <param name="eltStructurant"> "V4" ou "V8" </param>
		/// <param name="nbIteration"> nombre iteration pour "whiteTopHat" et "blackTopHat" </param>
		/// <returns> Image issue de la morpho </returns>
		_declspec(dllexport) CImageNdg morphologie(const std::string methode = "dilatation", const std::string eltStructurant = "V8", const int nbIteration = 1);

		/// <summary>
		/// filtrage par ecartTypeLocal
		/// </summary>
		/// <param name="methode"> "ecart type" </param>
		/// <param name="N"> taille element structurant N*N</param>
		/// <returns>Image filtré</returns>
		_declspec(dllexport) CImageNdg ecartTypeLocal(const std::string methode, const int N);

		/// <summary>
		/// labelisaiton V8
		/// </summary>
		/// <param name="nbComp">Nombre de composante connexe sans compter le fond</param>
		/// <returns></returns>
		_declspec(dllexport) CImageNdg labelImage(int* nbComp);



		/// <summary>
		/// transformée de Hough sur une image seuillée
		/// </summary>
		/// <param name="threshold">Seuillage des points maximums</param>
		/// <param name="diffLine">Savoir si toute les lignes à 255 ou alors avec un ID différent</param>
		/// <returns>l'image avec la transformée de Hough</returns>
		_declspec(dllexport) CImageNdg hough(int threshold = 150, bool diffLine = false);


#pragma endregion

};

#endif _IMAGE_NDG_