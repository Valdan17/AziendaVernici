#include "ProdottoVendibile.h"

/* Costruttori */
ProdottoVendibile::ProdottoVendibile(string nome, float prezzoKg) : ProdottoIndustriale(nome) {
	this->prezzoKg = prezzoKg;
}

ProdottoVendibile::ProdottoVendibile(string nome, float quantitaKg, float prezzoKg) : ProdottoIndustriale(nome, quantitaKg) {
	this->prezzoKg = prezzoKg;
}

/* Metodi per l'attributo prezzoKg */
float ProdottoVendibile::getPrezzoKg() const {
	return prezzoKg;
}

void ProdottoVendibile::setPrezzoKg(float prezzoKg) {
	this->prezzoKg = prezzoKg;
}