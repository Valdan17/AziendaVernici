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
	cout << "\nComposizione vernice " << nome << ":" << endl;
	for (elementoFormula elementoFormula : formula) {
		cout << " - " << elementoFormula.percentuale << "% " << elementoFormula.componente->getNome() << endl;
	}
}

void Vernice::stampaFormulaConQuantitaKg() const {
	cout << "\nComposizione vernice " << nome << ":" << endl;
	for (elementoFormula elementoFormula : formula) {
		cout << " - " << elementoFormula.percentuale << "% " << elementoFormula.componente->getNome() << " (" << elementoFormula.componente->getQuantitaKg() << " Kg disponibili)" << endl;
	}
}

/* Altri metodi */
void Vernice::stampa() const {
	cout << " - Vernice: " << nome << ", " << quantitaKg << "Kg" << ", " << prezzoKg << "Euro/Kg" << endl;
}

json Vernice::toJson() const {
	json j, componente;
	vector<json> colori, additivi;

	j["Nome"] = nome;
	j["PrezzoKg"] = prezzoKg;
	j["QuantitaKg"] = quantitaKg;
	
	for (const auto& elem : formula) {
		componente = elem.componente->toJson();
		componente["ID"] = elem.idComponente;
		componente["Percentuale"] = elem.percentuale;

		if (typeid(*elem.componente) == typeid(Colore)) {
			colori.push_back(componente);
		} else
			additivi.push_back(componente);
	}
	j["Formula"] = { {"Colori", colori}, {"Additivi", additivi} };
	
	return j;
}

string Vernice::whoAmI() const {
	return "Vernice";
}

bool Vernice::isProducibileInQuantitaKg(float quantitaDaProdurre) const {
	bool isVerniceProducibile = true;

	for (elementoFormula elementoFormula : formula) {
		float kgNecessari = (elementoFormula.percentuale/100.0f) * quantitaDaProdurre;
		float kgDisponibili = elementoFormula.componente->getQuantitaKg();
		Componente* componente = elementoFormula.componente;

		if (kgNecessari > kgDisponibili) {
			cout << " - Mancano " << kgNecessari - kgDisponibili << "Kg di " << componente->getNome() << endl;
			isVerniceProducibile = false;
		}
	}

	if (isVerniceProducibile) {
		for (elementoFormula elementoFormula : formula) {
			float kgNecessari = (elementoFormula.percentuale / 100.0f) * quantitaDaProdurre;
			float kgDisponibili = elementoFormula.componente->getQuantitaKg();
			Componente* componente = elementoFormula.componente;
			cout << " - Occorrono " << kgNecessari << "Kg di " << componente->whoAmI() << " " << componente->getNome() << " (su " << kgDisponibili << "Kg disponibili)" << endl;
		}
	}

	return isVerniceProducibile;
}
