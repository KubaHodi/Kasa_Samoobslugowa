#pragma once
#include "Article.h"

using namespace System::Collections::Generic;

namespace app {

    public ref class PrzykladoweDane abstract sealed
    {
    public:
        static List<ArtykulSpozywczy^>^ PobierzProdukty()
        {
            List<ArtykulSpozywczy^>^ produkty = gcnew List<ArtykulSpozywczy^>();

            produkty->Add(gcnew ArtykulSpozywczy("Chleb pszenny", 5.49, Jednostka::Sztuka, 0, "Pieczywo"));
            produkty->Add(gcnew ArtykulSpozywczy("Bu³ka kajzerka", 0.89, Jednostka::Sztuka, 0, "Pieczywo"));
            produkty->Add(gcnew ArtykulSpozywczy("Jab³ka", 4.99, Jednostka::Kilogram, 0, "Owoce"));
            produkty->Add(gcnew ArtykulSpozywczy("Ziemniaki", 2.99, Jednostka::Kilogram, 0, "Warzywa"));

            return produkty;
        }
    };

}