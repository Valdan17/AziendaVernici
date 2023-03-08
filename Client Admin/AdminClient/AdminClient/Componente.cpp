#include <iostream>

#include "Componente.h"

using namespace std;

/* Costruttori */

Componente::Componente(string nome) : ProdottoIndustriale(nome) {}

Componente::Componente(string nome, float quantitaKg) : ProdottoIndustriale(nome, quantitaKg) {}

Componente::Componente(Componente& c) : ProdottoIndustriale(c) {}

/* Altre funzioni */
void Componente::whoAmI() const{
	cout << ", un componente";
	ProdottoIndustriale::whoAmI();
}

json Componente::toJson() const {
	json j;
	j["Nome"] = nome;
	j["QuantitaKg"] = quantitaKg;
	return j;
}

/* Overload operatori */
Componente& Componente::operator=(const Componente& c) {
	nome = c.nome;
	quantitaKg = c.quantitaKg;
	return *this;
}

Componente& Componente::operator+=(const float quantitaKg) {
	this->quantitaKg += quantitaKg;
	return *this;
}