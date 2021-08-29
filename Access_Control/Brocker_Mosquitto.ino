void connerct_broker() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message

  //****open door relay****//

  if (String(topic) == "open/door") {
    digitalWrite(relay_door_pin, HIGH);
    delay(300);
    digitalWrite(relay_door_pin, LOW) ;
  }


  //****fingerprint enroll****//

  //finger print enroll send id for seve ene employee
  if (String(topic) == "enroll/getId") {
    id_new_fingerprint = messageTemp.toInt();
    getFingerprint1 = "run";
  }

  //control fingerprint and break while

  if (String(topic) == "enroll/program") {
    if (messageTemp == "enroll/program/stop") {
      getFingerprint1 = "stop";
      getFingerprint2 = "stop";
      moveToTable1 = "stop";
      moveToTable2 = "stop";
    }
    if (messageTemp == "enroll/program/run") {
      getFingerprint1 = "run";
    }
  }



  //use fingerprint touch tfor login ,log out normally open door ,noral status

  if (String(topic) == "state") {
    if (messageTemp == "state/openDoor/request") {
      oled_show_singerprintLogo();
    }
    else if (messageTemp == "state/login") {
      oled_show_login();

    }
    else  if (messageTemp == "state/logout") {
      oled_show_logout();
      //client.publish("esp32/led", "done");
    }
    else if (messageTemp == "state/normal") {
      oled_show_ArtaCo();
    }
    else if (messageTemp == "state/openDoor/ok") {
      oled_show_info();
    }
    else if (messageTemp == "state/openDoor/fail") {
      oled_show_noaccess();
    }
  }
  if (String(topic) == "state/fingerprint") {
    if (messageTemp == "state/fingerprint/run") {
      status_button = "fingerprintRun";
    }
    else if (messageTemp == "state/fingerprint/stop") {
      status_button = "fingerprintStop";
    }

  }

  //****neopixel***//

  if (String(topic) == "neopixel") {
    if (messageTemp == "neopixel/white") {
      neopixel_white();
    }
    else if (messageTemp == "neopixel/red") {
      neopixel_red();
    }
    else if (messageTemp == "neopixel/green") {
      neopixel_green();
    }
    else if (messageTemp == "neopixel/blue") {
      neopixel_blue();
    }
    else if (messageTemp == "neopixel/sleep") {
      neopixel_sleep();
    }

  }
  //****end neopixel****//
  //****give name and time employee****//

  if (String(topic) == "information/employee/name") {
    employee_name = messageTemp;
  }
  if (messageTemp == "information/employee/time") {
    employee_time = messageTemp;
  }
  //****end give name and time employee****//

}




void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      neopixel_blue();
      // Subscribe
      client.subscribe("esp32/output");
      client.subscribe("enroll/getId");
      client.subscribe("state");
      client.subscribe("state/fingerprint");
      client.subscribe("enroll/program");
      client.subscribe("open/door");



    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publish_broker() {

  // Temperature in Celsius
  temperature = 123;
  // Convert the value to a char array
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  Serial.print("Temperature: ");
  Serial.println(tempString);
  client.publish("esp32/temperature", tempString);

}
