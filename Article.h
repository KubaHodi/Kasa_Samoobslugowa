#pragma once
using namespace System;

namespace app {
	public enum Jednostka {
		Sztuka,
		Kilogram
	};

	public ref class ArtykulSpozywczy {

	public:
		property String^ Rodzaj;
		property double Cena;
		property Jednostka Jedn;
		property String^ Kategoria;
		property double Ilosc;

		ArtykulSpozywczy(String^ rodzaj, double cena, Jednostka jedn, double ilosc, String^ kategoria) {
			Rodzaj = rodzaj;
			Cena = cena;
			Jedn = jedn;
			Ilosc = ilosc;
			Kategoria = kategoria;
		}

		double Wartosc() {
			return Cena * Ilosc;
		}
	};
}