#pragma once 

#include <vector>
#include "ProdottoVendibile.h"
#include "Additivo.h"
#include "Colore.h"

struct elementoFormula {
	short percentuale;
	Componente* componente;
};

class Vernice : public ProdottoVendibile {
private:
	vector<elementoFormula> formula;
	
public:
	/* Costruttori */
	Vernice(string nome, float prezzoKg, vector<elementoFormula> formula);
	Vernice(string nome, float quantitaKg, float prezzoKg, vector<elementoFormula> formula);

	/* Metodi per l'attributo formula */
	vector<elementoFormula> getFormula() const;
	void setFormula(vector<elementoFormula> formula);
	void stampaFormula() const;

	/* Altri metodi */
	virtual void stampa() const;
	virtual json toJson() const;
	void whoAmI() const;
};

