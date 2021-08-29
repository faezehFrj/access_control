void control_buzzer_run() {
  digitalWrite(buzzer_pin, HIGH);
}
void control_buzzer_stop() {
  digitalWrite(buzzer_pin, LOW);
}

void control_buzzer_automatic() {
 digitalWrite(buzzer_pin, HIGH); 
 delay(1000);
 digitalWrite(buzzer_pin, LOW);

}
