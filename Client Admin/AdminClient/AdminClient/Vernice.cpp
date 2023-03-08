#include "Vernice.h"

/* Costruttori */
Vernice::Vernice(string nome, float prezzoKg, vector<elementoFormula> formula) : ProdottoVendibile(nome, prezzoKg) {
	this->formula = formula;
}

Vernice::Vernice(string nome, float quantitaKg, float prezzoKg, vector<elementoFormula> formula) : ProdottoVendibile(nome, quantitaKg, prezzoKg) {
	this->formula = formula;
}

/* Metodi per l'attributo formula */
vector<elementoFormula> Vernice::getFormula() const {
	return formula;
}

void Vernice::setFormula(vector<elementoFormula> formula) {
	this->formula = formula;
}

void Vernice::stampaFormula() const {
	cout << "Formula vernice: " << nome << endl;
	for (elementoFormula elemento : formula) {
		cout << " - " << elemento.percentuale << "% " << elemento.componente->getNome() << "\n" << endl;
	}
}

/* Altri metodi */
void Vernice::stampa() const {
	cout << "VERNICE: " << nome << ", quantita' disponibile: " << quantitaKg << "Kg" << endl;
}

json Vernice::toJson() const {
	json j;
	return j;
}

void Vernice::whoAmI() const {
	cout << "Sono una vernice";
	ProdottoVendibile::whoAmI();
}
