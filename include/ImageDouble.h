#pragma once

#ifndef _IMAGE_DOUBLE_
#define _IMAGE_DOUBLE_

#include <vector>
#include <string>

#include "ImageNdg.h"


class CImageDouble {

	private : 
		int              m_iHauteur;
		int              m_iLargeur; 
		std::string      m_sNom;
		double			 m_vMin; // valeur plus petite possible
		double           m_vMax; // valeur plus grande possible
		double*          m_pucPixel;

	public : 

#pragma region Constructeurs & Destructeurs

		// par défaut
		_declspec(dllexport) CImageDouble(); 

		// initialisation à 0
		_declspec(dllexport) CImageDouble(int hauteur, int largeur); 

		// ilage NDG en entrée, choix "normalise"/"cast"/"integrale1"/"integrale2"
		_declspec(dllexport) CImageDouble(const CImageNdg& im, const std::string& methode = "cast");  

		// image en entrée
		_declspec(dllexport) CImageDouble(const CImageDouble& im); 

		// destructeur
		_declspec(dllexport) ~CImageDouble();

#pragma endregion

#pragma region Get/Set

		_declspec(dllexport) int lireHauteur() const {
			return m_iHauteur;
		}

		_declspec(dllexport) int lireLargeur() const {
			return m_iLargeur;
		}

		_declspec(dllexport) double lireMin() const {
			return m_vMin;
		}

		_declspec(dllexport) double lireMax() const {
			return m_vMax;
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

		_declspec(dllexport) void ecrireMin(double min) {
			m_vMin = min;
		}

		_declspec(dllexport) void ecrireMax(double max) {
			m_vMax = max;
		}

		_declspec(dllexport) void ecrireNom(std::string nom) {
			m_sNom = nom;
		}

		_declspec(dllexport) int lireNbPixels() const {
			return m_iHauteur * m_iLargeur;
		}

#pragma endregion

#pragma region Surchage opérateur
		/// <summary>
		///  pouvoir accéder à un pixel par image(i)
		/// </summary>
		/// <param name="i"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) double& operator() (int i) const { 
		return m_pucPixel[i];
		}

		/// <summary>
		/// pouvoir accéder à un pixel par image(i,j)
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>valeur du pixel</returns>
		_declspec(dllexport) double& operator() (int i, int j) const { 
		return m_pucPixel[i*m_iLargeur+j];
		}

		/// <summary>
		/// opérateur copie image par imOut = imIn
		/// </summary>
		/// <param name="im">image a copier</param>
		/// <returns>image copié</returns>
		_declspec(dllexport) CImageDouble& operator=(const CImageDouble& im);

#pragma endregion

#pragma region Traitements

		/// <summary>
		/// distance au fond
		/// </summary>
		/// <param name="eltStructurant"> "V4", "V8" </param>
		/// <param name="valBord"></param>
		/// <returns></returns>
		_declspec(dllexport) CImageDouble distance(std::string eltStructurant = "V8", double valBord = 0);

		/// <summary>
		/// conversions
		/// </summary>
		/// <param name="methode">"defaut" "expansion"</param>
		/// <returns>image convertie</returns>
		_declspec(dllexport) CImageNdg toNdg(const std::string& methode = "defaut"); 

		/// <summary>
		/// Vecteur gradient 
		/// </summary>
		/// <param name="axe">"norme" (contours) ,"angle"</param>
		/// <returns></returns>
		_declspec(dllexport) CImageDouble vecteurGradient(const std::string& axe = "norme"); // choix "norme"/"angle"
		
		/// <summary>
		/// Filtrage
		/// </summary>
		/// <param name="methode"> "moyen" par défaut, "gaussien"</param>
		/// <param name="N"> taille element structurant N*N</param>
		/// <param name="sigma"> ecart type </param>
		/// <returns>image filtré</returns>
		_declspec(dllexport) CImageDouble filtrage(const std::string& methode = "moyen", int N = 3, double sigma = 0.5);

		/// <summary>
		/// pyramides
		/// </summary>
		/// <param name="hauteur"></param>
		/// <param name="tailleFiltre"></param>
		/// <param name="sigma"></param>
		/// <returns></returns>
		_declspec(dllexport) std::vector<CImageDouble> pyramide(int hauteur = 5, int tailleFiltre = 5, double sigma = 1);


		/// <summary>
		/// ecart type local
		/// </summary>
		/// <param name="im">image a filtré</param>
		/// <param name="N"> taille element structurant N*N </param>
		/// <returns>image filtré</returns>
		_declspec(dllexport) CImageNdg ecartTypeLocal(const CImageNdg& im, int N = 3);

		/// <summary>
		/// maxiLocaux de l'image double
		/// </summary>
		/// <returns>image avec maxiLocaux octant</returns>
		_declspec(dllexport) CImageNdg maxiLocauxOctant();

		/// <summary>
		/// Point d'intérêt de Harris 
		/// </summary>
		/// <param name="k">constante [0.01;0.06]</param>
		/// <param name="taille">taille noyau gaussien</param>
		/// <param name="sigma">écart-type de la distribution gaussienne</param>
		/// <returns></returns>
		_declspec(dllexport) CImageNdg harris(double k = 0.01, int taille = 3, double sigma = 1);
#pragma endregion

};

#endif 