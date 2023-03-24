#include "Colore.h"

using namespace std;

/* Costruttori */
Colore::Colore(string nome) : Componente(nome) {}

Colore::Colore(string nome, float quantitaKg) : Componente(nome, quantitaKg) {}

Colore::Colore(Colore& c) : Componente(c) {}

Colore::Colore(const Componente& c) : Componente(c) {}


/* Altri metodi */
void Colore::stampa() const {
	cout << " - Colore: " << getNome() << ", " << getQuantitaKg() << "Kg" << endl;
}

string Colore::whoAmI() const {
	return "Colore";
}

/* Overload operatori */
Colore& Colore::operator=(const Colore& c) {
	nome = c.nome;
	quantitaKg = c.quantitaKg;
	return *this;
}

Colore& Colore::operator+=(const float quantitaKg) {
	this->quantitaKg += quantitaKg;
	return *this;
}

bool Colore::operator==(const Colore& c) {
	return this->nome == c.getNome() && this->quantitaKg == c.getQuantitaKg();
}
   