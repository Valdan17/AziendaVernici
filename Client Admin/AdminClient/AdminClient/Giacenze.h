#pragma once

#include "Additivo.h"
#include "Colore.h"
#include "Vernice.h"

class Giacenze {
public:
	static map<int, Colore> colori;
	static map<int, Additivo> additivi;
	static map<int, Vernice> vernici;
};

