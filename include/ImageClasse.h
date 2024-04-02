// classe images segmentées, signatures

#pragma once

#ifndef _IMAGE_CLASSE_
#define _IMAGE_CLASSE_

#include <string>
#include <vector>

#include "ImageNdg.h"
#include "ImageCouleur.h"

#define EUCLIDEAN(a,b) ((a-b) * (a-b)) 
#define MAX_ITER 100

#pragma region Structures SIGNATURE_Ndg
typedef struct  {
	float  moyenne;
	int    surface;
} SIGNATURE_Ndg;
#pragma endregion

#pragma region Structures SIGNATURE_Couleur
typedef struct  {
	float  moyenne[3];
	int    surface;
} SIGNATURE_Couleur;
#pragma endregion

#pragma region Structures SIGNATURE_Forme
typedef struct {
	float		centreGravite_i;
	float		centreGravite_j;
	int			surface;
	std::string codeFreeman; // au sens V8
	int			premierPt_i; // premier point rencontré sens de parcours avant
	int			premierPt_j;
	int			rectEnglob_Hi; // coins supérieur gauche et inférieur droit
	int			rectEnglob_Hj;
	int			rectEnglob_Bi;
	int			rectEnglob_Bj;
	float		perimetre; // au sens V8
} SIGNATURE_Forme;
#pragma endregion

// définition classe Image Classe --> images étiquetées pour analyse objets, nuées dynamiques pour analyse régions

class CImageClasse {

	///////////////////////////////////////
	private : 
	///////////////////////////////////////

		int              m_iHauteur;
		int              m_iLargeur;
		std::string      m_sNom;
		long			 m_lNbRegions;

		unsigned long*   m_pucPixel;

	///////////////////////////////////////
	public : 
	///////////////////////////////////////
		
#pragma region Constructeurs & Destructeurs
		// par défaut
		_declspec(dllexport) CImageClasse(); 
		// initialisation à 0
		_declspec(dllexport) CImageClasse(int hauteur, int largeur); 
		
		/// <summary>
		/// Etiquetage
		/// </summary>
		/// <param name="im">img NDG à etiqueter</param>
		/// <param name="choix">"V8" ou "V4"</param>
		_declspec(dllexport) CImageClasse(const CImageNdg& im, std::string choix = "V8"); // objets

		/// <summary>
		/// re-étiquetage éventuel
		/// </summary>
		/// <param name="in">img </param>
		/// <param name="misAJour">"sans" -> Simple copie , "avec" -> Numerotation des classes </param>
		/// <param name="voisinage">"V8", "V4"</param>
		_declspec(dllexport) CImageClasse(const CImageClasse& in, std::string misAJour = "sans", std::string voisinage = "V8"); 

		/// <summary>
		/// clustering
		/// </summary>
		/// <param name="im">img NDG</param>
		/// <param name="nbClusters"></param>
		/// <param name="choix">"aleatoire" -> Germe aléatoire , "regulier" -> Germe intervalle régulier </param>
		_declspec(dllexport) CImageClasse(const CImageNdg& im, int nbClusters = 2, std::string choix = "aleatoire");  

		/// <summary>
		/// 
		/// </summary>
		/// <param name="im"></param>
		/// <param name="nbClusters"></param>
		/// <param name="choix"></param>
		/// <param name="espace"></param>
		/// <param name="plan"></param>
		_declspec(dllexport) CImageClasse(const CImageCouleur& im, int nbClusters = 2, std::string choix = "aleatoire", std::string espace = "hsv", int plan = 0);

		/// <summary>
		///  clustering contraint, germes a priori
		/// </summary>
		/// <param name="im"></param>
		/// <param name="germes"></param>
		_declspec(dllexport) CImageClasse(const CImageNdg& im, std::vector<unsigned char>* germes = NULL); 

		// destructeur
		_declspec(dllexport) ~CImageClasse(); 

#pragma endregion

#pragma region Sauvegarde
		/// <summary>
		/// sauvegarde au format bmp (attention : cast des informations car pertes potentielles)
		/// </summary>
		/// <param name="file">Nom du fichier</param>
		_declspec(dllexport) void sauvegarde(const std::string& file = ""); // sauvegarde data au format BMP avec cast des long en char
#pragma endregion

#pragma region Surcharge dopérateur
		/// <summary>
		/// pouvoir accéder à un pixel par image(i)
		/// </summary>
		/// <param name="i"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) unsigned long& operator() (int i) const { 
		return m_pucPixel[i];
		}

		/// <summary>
		/// pouvoir accéder à un pixel par image(i,j)
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) unsigned long& operator() (int i, int j) const { 
		return m_pucPixel[i*m_iLargeur+j];
		}

		/// <summary>
		/// opérateur copie image par imOut = imIn
		/// </summary>
		/// <param name="im">image à copier</param>
		/// <returns></returns>
		_declspec(dllexport) CImageClasse& operator=(const CImageClasse& im);
#pragma endregion
		
#pragma region Get & Set
		_declspec(dllexport) int lireHauteur() const { 
		return m_iHauteur;
		}

		_declspec(dllexport) int lireLargeur() const {
		return m_iLargeur;
		}

		_declspec(dllexport) int lireNbRegions() const {
		return m_lNbRegions;
		}

		_declspec(dllexport) std::string lireNom() const {
		return m_sNom;
		}

		_declspec(dllexport) int lireNbPixels() const {
		return m_iHauteur*m_iLargeur;
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

		_declspec(dllexport) void ecrireNbRegions(int nb) {
		m_lNbRegions = nb;
		}

#pragma endregion

#pragma region Signatures image & composantes
		/// <summary>
		/// Signature Image NDG
		/// </summary>
		/// <param name="img">Image NDG</param>
		/// <param name="enregistrementCSV">true pour enregistrer, false sinon</param>
		/// <returns>La liste de signature</returns>
		_declspec(dllexport) std::vector<SIGNATURE_Ndg> signatures(const CImageNdg& img, bool enregistrementCSV = false);

		/// <summary>
		/// Signature Image Couleur
		/// </summary>
		/// <param name="img">Image Couleur</param>
		/// <param name="enregistrementCSV">true pour enregistrer, false sinon</param>
		/// <returns>La liste de signature</returns>
		_declspec(dllexport) std::vector<SIGNATURE_Couleur> signatures(const CImageCouleur& img, bool enregistrementCSV = false);

		/// <summary>
		/// signatures forme pour Image_Ndg et Image_Couleur
		/// </summary>
		/// <param name="enregistrementCSV">true pour enregister, false sinon</param>
		/// <returns>La liste de signature des composantes</returns>
		_declspec(dllexport) std::vector<SIGNATURE_Forme> sigComposantesConnexes(bool enregistrementCSV = false);
#pragma endregion

#pragma region Affichage
		/// <summary>
		/// affichage chaque région NDG -> sa valeur moyenne
		/// </summary>
		/// <param name="img"></param>
		/// <param name="fondAPart"></param>
		/// <returns></returns>
		_declspec(dllexport) CImageNdg affichageMoyenne(const CImageNdg& img, bool fondAPart = true);

		/// <summary>
		/// affichage chaque région Couleur -> sa valeur moyenne
		/// </summary>
		/// <param name="img"></param>
		/// <param name="fondAPart"></param>
		/// <returns></returns>
		_declspec(dllexport) CImageCouleur affichageMoyenne(const CImageCouleur& img, bool fondAPart = true);
#pragma endregion

#pragma region Traitements
		/// <summary>
		/// filtrage selon critères taille, bords, etc
		/// </summary>
		/// <param name="methode">"taille", "bords"</param>
		/// <param name="taille">surface des composantes</param>
		/// <param name="miseAJour"> true -> met à jour la numérotation des labels, false -> ne met pas à jour la numérotation des labels </param>
		/// <returns>Image filtré</returns>
		_declspec(dllexport) CImageClasse filtrage(const std::string& methode = "taille", int taille=50, bool miseAJour = false);

		/// <summary>
		/// Morphologie
		/// </summary>
		/// <param name="methode">"erosion", "dilatation"</param>
		/// <param name="eltStructurant">"V4", "V8"</param>
		/// <returns>Image morpho</returns>
		_declspec(dllexport) CImageClasse morphologie(const std::string& methode = "erosion", const std::string& eltStructurant = "V8");
#pragma endregion
};

#endif _IMAGE_CLASSE_