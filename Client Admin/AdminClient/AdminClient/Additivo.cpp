#include "Additivo.h"

/* Costruttori*/ 
Additivo::Additivo(string nome) : Componente(nome) {}

Additivo::Additivo(string nome, float quantitaKg) : Componente(nome, quantitaKg) {}

Additivo::Additivo(Additivo& a) : Componente(a) {}

Additivo::Additivo(Componente& c) : Componente(c) {}

/* Altri metodi */
void Additivo::stampa() const {
	cout << " - Additivo: " << getNome() << ", " << getQuantitaKg() << "Kg" << endl;
}

string Additivo::whoAmI() const {
	return "Additivo";
}

/* Overload operatori */
Additivo& Additivo::operator=(const Additivo& c) {
	nome = c.nome;
	quantitaKg = c.quantitaKg;
	return *this;
}

Additivo& Additivo::operator+=(const float quantitaKg) {
	this->quantitaKg += quantitaKg;
	return *this;
}

bool Additivo::operator==(const Additivo& c) {
	return this->nome == c.getNome() && this->quantitaKg == c.getQuantitaKg();
}