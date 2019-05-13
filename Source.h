#ifndef SOURCE_H
#define SOURCE_H

#include <fstream>
#include <cstdio>
#include <cstdlib>

class Source
{
    std::string filename;
    std::ifstream file;

    int line_number;
    int pos_in_line;

    std::string current_line;
    std::string::iterator it;

    int current_ch;

public:
    Source(std::string filename)
    {
        file.open(filename.c_str());

        if(!file.is_open())
        {
            std::cout<< "Nie udalo sie otworzyc pliku!!!"<<std::endl;
            exit(1);
        }

        line_number = 0;
        pos_in_line = 0;

        getline(file, current_line);
        it = current_line.begin();
    }

    ~Source()
    {
        file.close();
    }

    void load_next_ch()
    {
        char ch;

        if(it == current_line.end()) // jak jest koniec linii
        {
            if(file.eof())
            {
                current_ch = EOF;
                return;
            }

            ch = '\n'; // jesli koniec stringa to daje znak konca linii i dalej przestawiam wskaznik nastepnego znaku

            do   // omijanie pustych linii
            {
                if(file.eof()) // na wypadek jakby plik konczyl sie wieloma pustymi linijkami
                {
                    current_ch = EOF;
                    return;
                }
                getline(file, current_line);
                if(current_line.length() == 0) // jesli puste to plusuj linie, jak nie puste to nizej sie splusuje
                    line_number++;
            }
            while(current_line.length() == 0);

            it = current_line.begin();

            line_number ++;
            pos_in_line = 0;
        }
        else if(it == current_line.begin()) // jak na poczatku to przesuwam tylko wskaznik nastepnego znaku
        {
            pos_in_line = 0;
            ch = *it;
            if(ch == '\t') pos_in_line += 7; // jesli pierwszy znak w linii to tab to dodaj wiecej
            it ++;
        }
        else
        {
            ch = *it;
            if(ch == '\t') pos_in_line += 8; // jesli napotkasz na tab to dodaj wiecej
            else pos_in_line++;
            it ++;
        }

        current_ch = ch;
    }

    int see_current_ch()
    {
        return current_ch;
    }

    int get_line_number()
    {
        return line_number;
    }

    int get_pos_in_line()
    {
        return pos_in_line;
    }

    std::string get_current_line()
    {
        return current_line;
    }
};


#endif

