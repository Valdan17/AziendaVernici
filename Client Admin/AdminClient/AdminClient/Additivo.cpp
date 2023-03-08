#include "Additivo.h"

/* Costruttori*/ 
Additivo::Additivo(string nome) : Componente(nome) {}

Additivo::Additivo(string nome, float quantitaKg) : Componente(nome, quantitaKg) {}

Additivo::Additivo(Additivo& a) : Componente(a) {}

/* Altri metodi */
void Additivo::stampa() const {
	cout << " - Additivo: " << getNome() << ", " << getQuantitaKg() << "Kg" << endl;
}

void Additivo::whoAmI() const {
	cout << "Sono un additivo";
	Componente::whoAmI();
}