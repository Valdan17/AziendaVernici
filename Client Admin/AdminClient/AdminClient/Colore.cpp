#include "Colore.h"

using namespace std;

/* Costruttori */
Colore::Colore(string nome) : Componente(nome) {}

Colore::Colore(string nome, float quantitaKg) : Componente(nome, quantitaKg) {}

Colore::Colore(Colore& c) : Componente(c) {}


/* Altri metodi */
void Colore::stampa() const {
	cout << " - Colore: " << getNome() << ", " << getQuantitaKg() << "Kg" << endl;
}

void Colore::whoAmI() const {
	Componente::whoAmI();
	cout << "Sono un colore";
}
   