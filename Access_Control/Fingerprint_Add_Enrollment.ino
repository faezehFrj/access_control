//void store_new_fingerprint(int id) {
//
//  Serial.println("Ready to enroll a fingerprint!");
//  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
//  //  id = readnumber();
//  if (id == 0) {// ID #0 not allowed, try again!
//    return;
//  }
//  Serial.print("Enrolling ID #");
//  Serial.println(id);
//
//  while (!  getFingerprintEnroll(id) );
//}

uint8_t finger_getImage(int id) {
  p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      result_enroll = "1";
      moveToTable1 = "run";
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      result_enroll = "2";
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      result_enroll = "3";
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      result_enroll = "4";
      break;
    default:
      Serial.println("Unknown error");
      result_enroll = "5";
      break;
  }
  result_enroll.toCharArray(result_enrollString, 2);
  client.publish("enroll/getImage", result_enrollString);
  if (moveToTable1 == "run") {
    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        result_enroll = "1";
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        result_enroll = "2";
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        result_enroll = "3";
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        result_enroll = "4";
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        result_enroll = "5";
        return p;
      default:
        Serial.println("Unknown error");
        result_enroll = "6";
        return p;
    }
    result_enroll.toCharArray(result_enrollString, 2);
    client.publish("enroll/convertToTemplate", result_enrollString);

    Serial.println("Remove finger");

    client.publish("enroll/RemoveFinger", "Remove finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    getFingerprint1 = "stop";
    getFingerprint2 = "run";
    moveToTable1 = "stop";


  }

}

//**********************************//

uint8_t same_finger_getImage(int id) {
  p = -1;
  p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("1");
      result_enroll = "Image taken";
      moveToTable2 = "run";
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      result_enroll = "2";
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      result_enroll = "3";
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      result_enroll = "4";
      break;
    default:
      Serial.println("Unknown error");
      result_enroll = "5";
      break;
  }
  result_enroll.toCharArray(result_enrollString, 2);
  client.publish("enroll/getImage", result_enrollString);

  // OK success!
  if (moveToTable2 == "run") {
    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        result_enroll = "1";
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        result_enroll = "2";
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        result_enroll = "3";
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        result_enroll = "4";
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        result_enroll = "5";
        return p;
      default:
        Serial.println("Unknown error");
        result_enroll = "6";
        return p;
    }
    result_enroll.toCharArray(result_enrollString, 2);
    client.publish("enroll/convertToTemplate", result_enrollString);
    getFingerprint2 = "stop";
    moveToTable2 = "stop";
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    result_enroll = "1";
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    result_enroll = "2";
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    result_enroll = "3";
    return p;
  } else {
    Serial.println("Unknown error");
    result_enroll = "4";
    return p;
  }
  result_enroll.toCharArray(result_enrollString, 2);
  client.publish("enroll/createModel", result_enrollString);

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    client.publish("enroll/store", "Stored!");
    p = 10;
    return p;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    client.publish("enroll/store", "Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    client.publish("enroll/store", "Could not store in that location");

    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    client.publish("enroll/store", "Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    client.publish("enroll/store", "Unknown error");
    return p;
  }
  result_enroll.toCharArray(result_enrollString, 2);
  client.publish("enroll/store", result_enrollString);
}
}
