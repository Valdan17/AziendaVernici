#include <iostream>

#include "Componente.h"

using namespace std;

/* Costruttori */

Componente::Componente(string nome) : ProdottoIndustriale(nome) {}

Componente::Componente(string nome, float quantitaKg) : ProdottoIndustriale(nome, quantitaKg) {}


/* Altre funzioni */
json Componente::toJson() const {
	json j;
	j["Nome"] = nome;
	j["QuantitaKg"] = quantitaKg;
	return j;
}