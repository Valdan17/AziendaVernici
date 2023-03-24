#include <iostream>
#include "ProdottoIndustriale.h"

using namespace std;

/* Costruttori */

ProdottoIndustriale::ProdottoIndustriale(string nome) {
	this->nome = nome;
	this->quantitaKg = 0.0f;
}

ProdottoIndustriale::ProdottoIndustriale(string nome, float quantitaKg) {
	this->nome = nome;
	this->quantitaKg = quantitaKg;
}


/* Metodi per l'attributo Nome */
string ProdottoIndustriale::getNome() const {
	return nome;
}

void ProdottoIndustriale::setNome(string nome) {
	this->nome = nome;
}


/* Metodi per l'attributo QuantitaKg */
float ProdottoIndustriale::getQuantitaKg() const {
	return quantitaKg;
}

void ProdottoIndustriale::setQuantitaKg(float quantitaKg) {
	this->quantitaKg = quantitaKg;
}