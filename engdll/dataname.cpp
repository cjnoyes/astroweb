#include "stdafx.h"
#include <stdio.h>
#include <string.h>

#include "paths.hpp"

char *data_name( char *name )
{
  return set_path( name, DATA_F );
}
