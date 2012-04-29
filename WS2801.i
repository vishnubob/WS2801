%module "WS2801"
%include "stdint.i"
%include "carrays.i"
%array_class(unsigned char, uint8_array);

%{
#include "WS2801.h"
%}

%include "WS2801.h"
