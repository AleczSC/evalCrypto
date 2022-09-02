#include <ByteConversions.h>

void byteToCharArray(byte *input, char *output, size_t length)
{
    for(int i = 0; i < length; i++)
      output[i] = (char) input[i];
    output[length] = '\0';
}

void byteToHexChar(byte *input, char *output, size_t lenIn, size_t lenOut)
{
/*NOTA: NO EJECTUAR ESTA FUNCION ANTES DE dh1 y dh2, 
interfiere con la generacion de llaves y no se por que.*/

  const char hexchars[] = "0123456789ABCDEF";
  
  for (uint8_t i = 0; i < lenIn; i++) 
  {
  
    output[i*2] = hexchars[(input[i] >> 4) & 0x0F];
    output[(i*2) + 1] = hexchars[input[i] & 0x0F];
  }
  output[lenOut] = '\0';
}

void byteToString(byte *input, String output, size_t length)
{
    char temp;

    output.remove(0, length + 1);
    for (int i = 0; i < length; i++)
    {
        temp = input[i];
        output += String(temp);
    }
    output += '\0';
}

void byteToHexString(byte *input, String output, size_t lenIn, size_t lenOut)
{
    /*NOTA: NO EJECTUAR ESTA FUNCION ANTES DE dh1 y dh2, 
interfiere con la generacion de llaves y no se por que.*/

  const char hexchars[] = "0123456789ABCDEF";
  
  for (uint8_t i = 0; i < lenIn; i++) 
  {
  
    output[i*2] = hexchars[(input[i] >> 4) & 0x0F];
    output[(i*2) + 1] = hexchars[input[i] & 0x0F];
  }
  output[lenOut] = '\0';
}

void charArrayToByte(char *input, byte *output, size_t length)
{
    for(int i = 0; i < length; i++)
        output[i] = (byte) input[i];
}

void hexCharToByte(char *input, byte *output, size_t lenIn, size_t lenOut)
{
    byte hn, ln;

    for (int i = 0; i < lenIn; i += 2)
    {
        if (input[i] <= 57)
            hn = (input[i] - 48) << 4;
        else
            hn = (input[i] - 55) << 4;

        if (input[i + 1] <= 57)
            ln = input[i + 1] - 48;
        else
            ln = input[i + 1] - 55;

        output[i / 2] = hn + ln;
    }
}

void strToByte(String input, byte *output, size_t lenIn, size_t lenOut)
{
  char temp[lenIn];                  //memcpy no funciona con string, así que se usa char temporal
  if (lenIn < lenOut)
  {
    for (int i = lenIn; i < lenOut; i++)
      *(output + i) = ' ';          //Agrega espacios en blanco en caso de que el string sea menor al arreglo de salida 
  }

  else if (lenIn > lenOut)
  lenIn = lenOut;                    //Recorta la cadena al tamaño de salida

  for (int j = 0; j < lenIn; j++)
    *(temp + j) = input[j];
  memcpy(output, temp, lenIn);     //Convierte str a byte
}

void hexStrToByte(String input, byte *output, size_t lenIn, size_t lenOut)
{
    byte hn, ln;

    if((lenIn / 2) < lenOut)
        for (int i = lenIn / 2; i < lenOut; i++)
            output[i] = ' ';
    
    else if ((lenIn / 2) > lenOut)
        lenIn = lenOut * 2;


    for (int i = 0; i < lenIn; i += 2)
    {
        if (input[i] <= 57)
            hn = (input[i] - 48) << 4;
        else
            hn = (input[i] - 55) << 4;

        if (input[i + 1] <= 57)
            ln = input[i + 1] - 48;
        else
            ln = input[i + 1] - 55;

        output[i / 2] = hn + ln;
    }

}