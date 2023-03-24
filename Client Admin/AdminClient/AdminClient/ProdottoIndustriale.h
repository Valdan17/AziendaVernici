#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class ProdottoIndustriale {
protected:
	string nome;
	float quantitaKg;

public:
	/* Costruttori */
	ProdottoIndustriale(string nome);
	ProdottoIndustriale(string nome, float quantitaKg);

	/* Metodi per l'attributo nome */
	string getNome() const;
	void setNome(string nome);

	/* Metodi per l'attributo quantitaKg */
	float getQuantitaKg() const;
	void setQuantitaKg(float quantitaKg);

	/* Altri metodi */
	virtual void stampa() const = 0;
	virtual json toJson() const = 0;
	virtual string whoAmI() const = 0; 
};

