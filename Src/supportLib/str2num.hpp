/*
IOController-FW - the firmware for STM32 microcontrollers which allows controls IO(GPIO) via USB 
Copyright (C) 2017 Stepan Hamouz, s.hamouz@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STR2NUM_HPP
#define STR2NUM_HPP

#include "stdint.h"

template <class T>
class str2numBase
{
  public:
    static float str2float(char *a, uint16_t len);
    static uint32_t str2uint(char *a, uint16_t len, uint8_t soust);
    static uint8_t  str2uintEx(uint32_t *retValue, char *a, uint16_t len, uint8_t soust);
    static int32_t  str2int(char *a, uint16_t len, uint8_t soust);
    static uint8_t  str2intEx(int32_t *retValue, char *a, uint16_t len, uint8_t soust);
        
  protected:

  private:
};

template <class T>
float str2numBase <T>::str2float(char *a, uint16_t len)
{
    uint16_t i = 0;
    uint32_t val = 0;
    int8_t sign = 1;
  
    float output = 0;
    uint16_t exponent = 0;

    if(a[0] == '-')
    {
      sign = -1;
      i=1;
    }
    
    
    for(i;i<len;i++)
    {   
      if(exponent > 0)
      {
          exponent++;
      }
      if((a[i] >= '0') && (a[i] <= '9'))
      {
        val = val*10 + a[i]-'0';
      }
      else if(a[i] >= '.')
      {
        exponent = 1;
      }
      else
      {
        return 0;
      }
    }

    output = val;

    i = 1;
    while(i < exponent)
    {
       output *= 0.1;
       i++;
    }
    
    output *= sign;

    return output;
}


template <class T>
uint8_t str2numBase <T>::str2uintEx(uint32_t *retValue, char *a, uint16_t len, uint8_t soust)
{
  uint16_t i = 0;
  uint16_t addVal = 0;
  uint32_t val = 0;

  for(i=0;i<len;i++)
  {   
    if((a[i] >= '0') && (a[i] <= '9'))
    {
      addVal = a[i]-'0';
    }
    else if((a[i] >= 'a') && (a[i] <= 'z')) // a menci nez..
    {
      addVal = a[i]-'a'+10;
    }
    else if((a[i] >= 'A') && (a[i] <= 'Z'))  // a menci nez..
    {
      addVal = a[i]-'A'+10;
    }
    else
    {
      return 0;
    }
     val = val*soust + addVal;
  }

  *retValue = val;    
  return 1;
}

template <class T>
uint32_t str2numBase <T>::str2uint(char *a, uint16_t len, uint8_t soust)
{
  uint32_t val = 0;
  (void)str2uintEx(&val, a, len, soust);
  return val;
}


template <class T>
int32_t str2numBase <T>::str2int(char *a, uint16_t len, uint8_t soust)
{
  int32_t ret = 0;
  
  if(a[0] == '-')
  {
    (void)str2uintEx(reinterpret_cast<uint32_t*>(&ret), &a[1], len-1, soust);
    ret *= -1;
  }
  else
  {
    (void)str2uintEx(reinterpret_cast<uint32_t*>(&ret), a, len, soust);
  }
  
  return ret;
}

template <class T>
uint8_t str2numBase <T>::str2intEx(int32_t *retValue, char *a, uint16_t len, uint8_t soust)
{
  uint8_t ret = 0;
  
  if(a[0] == '-')
  {
    ret = str2uintEx(reinterpret_cast<uint32_t*>(retValue), &a[1], len-1, soust);
    (*retValue) *= -1;
  }
  else
  {
    ret = str2uintEx(reinterpret_cast<uint32_t*>(retValue), a, len, soust);
  }
  
  return ret;
}



template <class T>
class str2num : public str2numBase<T>
{
    public:
      str2num();
      static T getNum(char *str, uint8_t size, uint8_t radix);
      static uint8_t getNumEx(T *retValue, char *str, uint8_t size, uint8_t radix);
      
    protected:


    private:
      //uint8_t lastStatus;
};


template <>  // int32_t
int32_t str2num<int32_t>::getNum(char *str, uint8_t size, uint8_t radix){
  return str2int(str, size, radix);
}

template <>  // int32_t
uint8_t str2num<int32_t>::getNumEx(int32_t *retValue, char *str, uint8_t size, uint8_t radix){
  return str2intEx(retValue, str, size, radix);
}

template <>  // int16_t
int16_t str2num<int16_t>::getNum(char *str, uint8_t size, uint8_t radix){
  return str2int(str, size, radix);
}

template <>  // int16_t
uint8_t str2num<int16_t>::getNumEx(int16_t *retValue, char *str, uint8_t size, uint8_t radix){
  return str2intEx(reinterpret_cast<int32_t*>(retValue), str, size, radix);
}

template <> // int8_t
int8_t str2num<int8_t>::getNum(char *str, uint8_t size, uint8_t radix){
  return str2int(str, size, radix);
}

template <>  // int8_t
uint8_t str2num<int8_t>::getNumEx(int8_t *retValue, char *str, uint8_t size, uint8_t radix){
  return str2intEx(reinterpret_cast<int32_t*>(retValue), str, size, radix);
}


template <> // uint32_t
uint32_t str2num<uint32_t>::getNum(char *str, uint8_t size, uint8_t radix){
  return str2uint(str, size, radix); 
}

template <> // uint32_t
uint8_t str2num<uint32_t>::getNumEx(uint32_t *retValue, char *str, uint8_t size, uint8_t radix){
  return str2uintEx(retValue, str, size, radix); 
}

template <> // uint16_t
uint16_t str2num<uint16_t>::getNum(char *str, uint8_t size, uint8_t radix){
  return str2uint(str, size, radix);
}

template <> // uint16_t
uint8_t str2num<uint16_t>::getNumEx(uint16_t *retValue, char *str, uint8_t size, uint8_t radix){
  return str2uintEx(reinterpret_cast<uint32_t*>(retValue), str, size, radix); 
}

template <> // uint8_t
uint8_t str2num<uint8_t>::getNum(char *str, uint8_t size, uint8_t radix){
  return str2uint(str, size, radix);

}

template <> // uint8_t
uint8_t str2num<uint8_t>::getNumEx(uint8_t *retValue, char *str, uint8_t size, uint8_t radix){
  return str2uintEx(reinterpret_cast<uint32_t*>(retValue), str, size, radix); 
}


template <> // float
float str2num<float>::getNum(char *str, uint8_t size, uint8_t radix){
  return str2float(str, size);
}



#endif // STR2NUM_HPP
