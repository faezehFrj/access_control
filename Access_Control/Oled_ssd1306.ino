void oled_show_ArtaCo()
{
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(20, 26, "Electronic");
  display.drawString(22, 45, "Company");
  display.display();
}

void oled_show_info()
{
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(1, 26, "client_name");
  display.drawString(1, 45, "client_time");
  display.display();

}

void oled_show_login()
{
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(35, 30, "Log in");
  display.display();
}

void oled_show_logout() {
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(42, 30, "EXIT");
  display.display();
}

void oled_show_singerprintLogo() {
    display.clear();
   display.flipScreenVertically();//tasaviro bar migardone
  display.drawXbm(0,0,128,64,pic_fingerprint);
  display.display();
}

void oled_show_noaccess() {
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(10, 35, "NO Access!");
  display.display();
}

void oled_show_access() {
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(10, 26, "WELLCOME");
  display.display();
}
void oled_show_enroll_success() {
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(10, 26, "Stored");
  display.display();
}
void oled_show_enroll_fail() {
  display.clear();
  display.drawString(40, 0, "ARTA");
  display.drawString(10, 26, "Failed");
  display.display();
}
