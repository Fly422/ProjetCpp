#pragma once

#ifndef _IMAGE_COULEUR_
#define _IMAGE_COULEUR_

#include <vector>
#include <string>
#include <algorithm>

#include "ImageNdg.h"

#define MIN3(x,y,z)  ((y) <= (z) ? ((x) <= (y) ? (x) : (y)) : ((x) <= (z) ? (x) : (z)))
#define MAX3(x,y,z)  ((y) >= (z) ? ((x) >= (y) ? (x) : (y)) : ((x) >= (z) ? (x) : (z)))

class CImageCouleur {

	private:
		int              m_iHauteur;
		int              m_iLargeur;
		std::string      m_sNom;
		unsigned char*   m_pucData;
		unsigned char**  m_ppucPixel;

	public:

#pragma region Constructeurs & Destructeurs
		//par defaut
		_declspec(dllexport) CImageCouleur();
	
		// si -1 alors non pixels non initialisés
		_declspec(dllexport) CImageCouleur(int hauteur, int largeur, int valR = -1, int valV = -1, int valB = -1); 
	
		//par fichier
		_declspec(dllexport) CImageCouleur(const std::string nom);
	
		//par image couleur
		_declspec(dllexport) CImageCouleur(const CImageCouleur& im);
	
		//par image NDG
		_declspec(dllexport) CImageCouleur(const CImageNdg& im);
	
		// par plan RGB
		_declspec(dllexport) CImageCouleur(const CImageNdg& pR, const CImageNdg& pG, const CImageNdg& pB);
	
		// img binaire de préférence contours
		_declspec(dllexport) CImageCouleur(const CImageNdg& im, const CImageNdg& mask, int R = 255, int G = 0, int B = 0); 
	
		// img binaire de préférence contours
		_declspec(dllexport) CImageCouleur(const CImageCouleur& im, const CImageNdg& mask, int R = 255, int G = 0, int B = 0); 

		// destructeur
		_declspec(dllexport) ~CImageCouleur(); 

#pragma endregion

#pragma region Get/Set

		_declspec(dllexport) int lireHauteur() const {
			return m_iHauteur;
		}

		_declspec(dllexport) int lireLargeur() const {
			return m_iLargeur;
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

		_declspec(dllexport) void ecrireNom(std::string nom) {
			m_sNom = nom;
		}

		_declspec(dllexport) int lireNbPixels() const {
			return m_iHauteur * m_iLargeur;
		}

#pragma endregion

#pragma region Surcharge operateur
		/// <summary>
		/// pouvoir accéder à un pixel par image(i,j)[plan]
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) unsigned char*& operator() (int i, int j) const {
			return m_ppucPixel[i*m_iLargeur + j];
		}

		/// <summary>
		/// pouvoir accéder à un pixel par image(i)[plan]
		/// </summary>
		/// <param name="i"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) unsigned char*& operator() (int i) const {
			return m_ppucPixel[i];
		}

		/// <summary>
		/// opérateur copie image par imOut = imIn
		/// </summary>
		/// <param name="im">Image à copier</param>
		/// <returns>Image copié</returns>
		_declspec(dllexport) CImageCouleur  &operator=(const CImageCouleur& im);

#pragma endregion

#pragma region Sauvegarde
		// sauvegarde au format bmp
		_declspec(dllexport) void sauvegarde(const std::string file = "");

#pragma endregion

#pragma region Histogramme

		/// <summary>
		/// histogramme 3 x plans 
		/// </summary>
		/// <param name="enregistrementCSV">true pour avoir un CSV ou false sinon</param>
		/// <returns>Histogramme des 3 plans</returns>
		_declspec(dllexport) std::vector<unsigned long> histogramme(bool enregistrementCSV = false); 

#pragma endregion

#pragma region Plan
		/// <summary>
		/// gestion des plans
		/// </summary>
		/// <param name="choix"> "0" -> R ou H, "1" -> V ou S, "2" -> B ou V et "3" -> plan de luminance d'où les poids fct de l'illuminant </param>
		/// <param name="poidsRouge"></param>
		/// <param name="poidsVert"></param>
		/// <param name="poidsBleu"></param>
		/// <returns>Le plan choisit</returns>
		_declspec(dllexport) CImageNdg plan(int choix = 3, double poidsRouge = 1. / 3., double poidsVert = 1. / 3., double poidsBleu = 1. / 3.); 
	
		/// <summary>
		/// mise a jour plan
		/// </summary>
		/// <param name="choix"> "0" -> R ou H, "1" -> V ou S, "2" -> B ou V et "3" -> plan de luminance d'où les poids fct de l'illuminant "</param>
		/// <param name="plan">Le nouveau plan</param>
		/// <returns>Image avec plan mis a jour</returns>
		_declspec(dllexport) CImageCouleur& miseAJourPlan(int choix, const CImageNdg& plan);
#pragma endregion

#pragma region Traitement
		/// <summary>
		/// changements d'espace couleur 
		/// </summary>
		/// <param name="methode"> "HSV" </param>
		/// <returns>Image dnas un autre espace couleur</returns>
		_declspec(dllexport) CImageCouleur conversion(const std::string methode = "HSV") const; 

		/// <summary>
		/// transformation
		/// </summary>
		/// <param name="methode">"expansion" sur [0,255] uniquement sur chaque plan, "egalisation"</param>
		/// <returns>Image issue de la transfo</returns>
		_declspec(dllexport) CImageCouleur transformation(const std::string methode = "expansion");
#pragma endregion

#pragma region Evaluation
		/// <summary>
		/// Detecteur de peau HSV
		/// </summary>
		/// <param name="methode"> RGB ou HSV</param>
		/// <returns>Image binaire de la détection</returns>
		_declspec(dllexport) CImageNdg detecteurPeau(const std::string methode = "HSV");
#pragma endregion

};

#endif _IMAGE_COULEUR_