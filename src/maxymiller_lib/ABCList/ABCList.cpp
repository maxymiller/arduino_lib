#include <maxymiller_lib/ABCList.h>

String ABCList::list(int numero) {
    switch (numero) {
    case 1:
        return "a";
        break;
    case 2:
        return "b";
        break;
    case 3:
        return "c";
        break;
    case 4:
        return "d";
        break;
    case 5:
        return "e";
        break;
    case 6:
        return "f";
        break;
    case 7:
        return "g";
        break;
    case 8:
        return "h";
        break;
    case 9:
        return "i";
        break;
    case 10:
        return "j";
        break;
    case 11:
        return "k";
        break;
    case 12:
        return "l";
        break;
    case 13:
        return "m";
        break;
    case 14:
        return "n";
        break;
    case 15:
        return "o";
        break;
    case 16:
        return "p";
        break;
    case 17:
        return "q";
        break;
    case 18:
        return "r";
        break;
    case 19:
        return "s";
        break;
    case 20:
        return "t";
        break;
    case 21:
        return "u";
        break;
    case 22:
        return "v";
        break;
    case 23:
        return "w";
        break;
    case 24:
        return "x";
        break;
    case 25:
        return "y";
        break;
    case 26:
        return "z";
        break;

    case 27:
        return "A";
        break;
    case 28:
        return "B";
        break;
    case 29:
        return "C";
        break;
    case 30:
        return "D";
        break;
    case 31:
        return "E";
        break;
    case 32:
        return "F";
        break;
    case 33:
        return "G";
        break;
    case 34:
        return "H";
        break;
    case 35:
        return "I";
        break;
    case 36:
        return "J";
        break;
    case 37:
        return "K";
        break;
    case 38:
        return "L";
        break;
    case 39:
        return "M";
        break;
    case 40:
        return "N";
        break;
    case 41:
        return "O";
        break;
    case 42:
        return "P";
        break;
    case 43:
        return "Q";
        break;
    case 44:
        return "R";
        break;
    case 45:
        return "S";
        break;
    case 46:
        return "T";
        break;
    case 47:
        return "U";
        break;
    case 48:
        return "V";
        break;
    case 49:
        return "W";
        break;
    case 50:
        return "X";
        break;
    case 51:
        return "Y";
        break;
    case 52:
        return "Z";
        break;

    default:
        return "0";
        break;
    }
}

String ABCList::listABC(int numero) {
    if (numero == 0) {
        return "0";
    }

    bool negativo = numero < 0;
    unsigned int n = negativo
        ? (unsigned int)(-(long long)numero)
        : (unsigned int)numero;

    String resultado = "";

    while (n > 0) {
        n--;
        int resto = n % ABC_NUMEOR;
        resultado = list(resto + 1) + resultado;
        n /= ABC_NUMEOR;
    }

    return (negativo ? "-" : "") + resultado;
}

String ABCList::listInt(String abc) {
    if (abc.length() == 0) {
        return "0";
    }

    bool negativo = abc.charAt(0) == '-';
    int inicio = negativo ? 1 : 0;

    if (inicio == abc.length()) {
        return "0";
    }

    long long resultado = 0;

    for (int i = inicio; i < abc.length(); i++) {
        char caractere = abc.charAt(i);
        int valor;

        if (caractere >= 'a' && caractere <= 'z') {
            valor = caractere - 'a' + 1;
        } else if (caractere >= 'A' && caractere <= 'Z') {
            valor = caractere - 'A' + 27;
        } else {
            return "0";
        }

        resultado = resultado * ABC_NUMEOR + valor;
    }

    if (negativo) {
        resultado = -resultado;
    }

    return String(resultado);
}
