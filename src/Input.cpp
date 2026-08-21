//20-8-2026: 

#include "../include/Input.h"

#include <iostream>
#include <limits>

using namespace std;

// Helperfunctie 1
// We geven een referentie door naar prompt en die mag niet worden aangepast.
double vraagDouble(const std::string& prompt)
{
    double waarde;

    std::cout<<prompt;

    //Zolang het niet lukt een geldige double uit de invoer te halen, blijf opnieuw vragen
    while(!(cin>>waarde))
    {
        //Melding ongeldige invoer
        cout<<"Ongeldige invoer. Probeer opnieuw: ";
        //Maak cin leeg: dat is de foutstatus
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        //Opruimactie van de foute invoer: Gooi alles weg tot de eerstvolgende enter.
        //BELANGRIJK: zonder ignore zou de input nog in de buffer staan.
    }
    return waarde;

}

// Helperfunctie 2
int vraagInt(const string& prompt)
{
    int waarde;

    cout<<prompt;

    while(!(cin>>waarde))
    {
        cout<<"Ongeldige invoer. Probeer opnieuw: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return waarde;
}


