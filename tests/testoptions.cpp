#include <stdio.h>
#include "setup.hpp"
#include "OptionsReader.h"
#include <memory.h>

int main( int argc, char *argv[]) {
   Options options;
   
   memset(&options.getOpt(),0,sizeof(OPTIONS));
   FILE *stream = fopen("test.conf","r");
   if (stream == NULL) {
      printf("stream is NULL\n");
   }
   OptionsReader *rdr = new OptionsReader(stream);
   int val = rdr->readOptions(options);
   printf("%d flags sb 246\n", (int)options.getFlags());
   printf("%d house mode sb 12\n", (int)options.getHouseMode());
   printf("%d rect method sb 3\n", (int)options.getDefRectMethod());
   printf("%d code sb 21\n", options.getChartCode());
   printf("%d return sb 1\n", val);
}