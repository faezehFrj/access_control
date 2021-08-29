// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
//Ask the sensor to convert image to feature template.
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK){
  
    Serial.print("Found ID #"); Serial.print(finger.fingerID); 
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    client.publish("search/find", "access");
  }

  if(p==FINGERPRINT_NOTFOUND){
     Serial.print("your finger did not find");
     client.publish("search/NOTfind", "denied");
  }
  
  // found a match!
  
  return finger.fingerID; 
}
