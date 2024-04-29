#include "sd_card.hpp"
#include <SPI.h>
#include <SD.h>


File myFile;
const int CS = 5;


int sd_card_init( void )
{
  Serial.println("Initializing SD card...");
  if(!SD.begin(CS))
  { 
    Serial.println("Initialization failed!");
    return -1;
  }
  Serial.println("Initialization done.");

  for(int i = 0; i < 3; i++) {
    digitalWrite(2, HIGH);
    delay(50); 
    digitalWrite(2, LOW);
    delay(50);
  }

  return 0;
}


uint32_t sd_card_create_new_file( void )
{  
  uint32_t file_idx = 0;
  char path[24] = "/esp32_log_0.txt";
  
  // check which index is not already in use
  while (true == SD.exists(path)) {
    file_idx++;
    if (999 == file_idx) { 
      Serial.println("Reached maximum index.");
      break; 
    }
    sprintf(path, "/esp32_log_%lu.txt", file_idx);
  }
  
  Serial.printf("Successfully created new file %s on SD card.\n\r", path);

  // open filestream
  myFile = SD.open(path, FILE_WRITE);

  // print data information as first lines
  myFile.println("Time since POR | Temperature | Humidity | CO2-Concentration");
  myFile.println("-----------------------------------------------------------");

  // close filestream
  myFile.close();

  for(int i = 0; i < 3; i++) {
    digitalWrite(2, HIGH);
    delay(50); 
    digitalWrite(2, LOW);
    delay(50);
  }

  return file_idx; 
}


int sd_card_append_to_file(const char* path, const char* str)
{
  // open filestream
  myFile = SD.open(path, FILE_APPEND);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Writing to %s... ", path);
  myFile.println(str);
  myFile.close();
  Serial.println("Writing completed.");

  return 0;
}

int sd_card_append_to_log_file(uint32_t file_idx, const char* str)
{
  // open filestream
  char path[24]; 
  sprintf(path, "/esp32_log_%lu.txt", file_idx);
  myFile = SD.open(path, FILE_APPEND);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Writing to %s... ", path);
  myFile.println(str);
  myFile.close();
  Serial.println("Writing completed.");

  return 0;
}


int sd_card_read_from_file(const char* path)
{
  myFile = SD.open(path);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Reading file from %s \n", path);
  
  // read entire file
  while(myFile.available()) 
  { 
    Serial.write(myFile.read()); 
  }

  return 0;
}