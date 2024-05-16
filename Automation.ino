#include<WiFi.h>
#include <WebServer.h>
#include<dht11.h>

const char *username = "Redmi 9 Prime";
const char *password = "12345678";

WebServer server(80); 

void show_temp_humi(void);
int calc_fan_efficiency(void);
int show_fan_efficiency(void);
int turn_on_1(void);
int turn_on_2(void);
int turn_on_3(void);
int turn_on_4(void);
int calc_percent(void);
void display(void);
int minimum_temp=30;
int medium_temp1=35;
int medium_temp2=40;
int maximum_temp=45 ;
int tempin=18;
int power_for_sensor=4;
int power_for_relay=5;
int fan_speed;
int fan_state = 0;
dht11 dht;
void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600 , SERIAL_8N1 , 16 , 17);
    WiFi.begin(username, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi....");
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.localIP());

  server.begin();
  server.on("/",login_page);
  server.on("/turnOff" , turn_off);
  server.on("/turnOn" , turn_on);
   
  pinMode(power_for_relay,OUTPUT);
  pinMode(power_for_sensor,OUTPUT);
  digitalWrite(power_for_sensor,HIGH);
}

void loop()
{
  server.handleClient();
  dht.read(tempin);
  //dht.temperature=28;             //Testing purpose
  //Serial.println(WiFi.status() == WL_CONNECTED);
  show_temp_humi();
  
  if(fan_state){
    //fan_speed=//calc_fan_efficiency();
    //display(show_fan_efficiency());
    digitalWrite(5 , HIGH);
  }
  else{
    //fan_speed = fan_state;
    digitalWrite(power_for_relay,LOW);
  }
  Serial.print("IP to request ESP32 Web server :");
  Serial.println(WiFi.localIP()); 
}
void show_temp_humi()
{
  Serial.println("----------------------------");
  Serial.print("|Temperature   ===> ");
  Serial.print((float)dht.temperature,2);
  Serial.println("°C |");
  Serial.print("|Humidity      ===> ");
  Serial.print(dht.humidity);
  Serial.println("% RH  |");
}
int calc_fan_efficiency()
{
  if(dht.temperature<minimum_temp)
  {
    return 0;
  }
  else if(dht.temperature>=minimum_temp && dht.temperature<medium_temp1)
  {
    return 1;
  }
  else if(dht.temperature>=medium_temp1 && dht.temperature<medium_temp2)
  {
    return 2;
  }
  else if(dht.temperature>=medium_temp2 && dht.temperature<maximum_temp)
  {
    return 3;
  }
  else
  {
    return 4;
  }
}
int show_fan_efficiency()
{
  switch(fan_speed)
  {
    case 0: 
        return 0;
    case 1: 
        return turn_on_1();
    case 2: 
        return turn_on_2();
    case 3: 
        return turn_on_3();
    case 4: 
        return turn_on_4();
  }
}
int turn_on_1()
{
   digitalWrite(power_for_relay,HIGH);
   delay(100);
   digitalWrite(power_for_relay,LOW);
   delay(200);
   return calc_percent();
}
int turn_on_2()
{
   digitalWrite(power_for_relay,HIGH);
   delay(500);
   digitalWrite(power_for_relay,LOW);
   delay(200);
   return calc_percent();
}
int turn_on_3()
{
   digitalWrite(power_for_relay,HIGH);
   delay(1000);
   digitalWrite(power_for_relay,LOW);
   delay(200);
   return calc_percent();
}
int turn_on_4()
{
   digitalWrite(power_for_relay,HIGH);
   return calc_percent();
}
int calc_percent()
{
  if(dht.temperature<minimum_temp)
  {
    return 0;
  }
  else if(dht.temperature>=minimum_temp && dht.temperature<medium_temp1)
  {
    return 25;
  }
  else if(dht.temperature>=medium_temp1 && dht.temperature<medium_temp2)
  {
    return 50;
  }
  else if(dht.temperature>=medium_temp2 && dht.temperature<maximum_temp)
  {
    return 75;
  }
  else
  {
    return 100;
  }
}
void display(int percent)
{
  Serial.print("|Fan Regulator ===> ");
  Serial.print(fan_speed);
  Serial.println("       |");
  Serial.print("|Fan Speed     ===> ");
  Serial.print(percent);
  Serial.println("%     |");
  Serial.println("----------------------------");
  Serial.println();
}


void login_page(){
  String appendhumi = String(dht.humidity);
  String appendtemp = String(dht.temperature);
  //String fanefficiency = String(calc_percent());
  String login_html = "<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>Log in</title> <style> @import url('https://fonts.googleapis.com/css2?family=Ubuntu:ital,wght@0,300;0,400;0,500;0,700;1,300;1,400;1,500;1,700&display=swap'); body{ margin: 0; } .out-container{ background-image: url('https://res.cloudinary.com/dredwklnr/image/upload/v1713069086/raw-food-with-water-drops_q4p5li.jpg'); height:100vh; background-size: cover; display: flex; align-items: center; justify-content: center; } .containers{ color : #ffffff; font-family: 'Ubuntu', sans-serif; border-radius: 20px; backdrop-filter: blur(12px); padding: 30px; } .font-size-place{ font-weight: 400; font-size: 25px; } .font-size-change{ font-weight: 400; font-size: medium; } .hr-color{ color: #ffffff; } .hr{ border-style: solid; border-width: 0.5px; border-color: gray; } .temp{ font-weight: bolder; font-size: 150px; } span{ font-size:125px; font-weight: 400; } /* .text-second{ color: #a4a4a4; } */ .celsious:hover,.farenheit:hover{ cursor: pointer; }.form{ display: flex; justify-content: center; align-items: center; height: 100vh;}.forms{ text-align: center; border-style: solid; border-color: white; border-width: 2px; padding-left: 75px; padding-right:75px; padding-top: 30px; padding-bottom: 30px; border-radius: 16px; background: rgba(255, 255, 255, 0.3); display: flex; flex-direction: column; justify-content: center; align-items: center;}.head{ margin:0px; color: black; font-weight: bold;}.outer-container{ font-family: 'Roboto'; background-image: url('https://res.cloudinary.com/dredwklnr/image/upload/v1712228241/vecteezy_abstract-blue-fluid-wave-background_17798503_dnymt1.jpg'); height:100vh; background-size: cover;}.but{ margin-top: 20px; color: #ffffff; border-radius: 5px; width:50%; border-width: 0px; background-color:#0e0252; padding: 5px;}.para{ font-size: 15px; color:rgb(204, 202, 202); font-weight: bold;}.user ,.pass{ height:25px !important; outline: none; border-width: 0px; border-radius: 10px; width: 80%; margin:20px; padding: 5px;}.anchors{ margin: 0px; width: 80%; margin-left: auto; margin-right: auto; text-align: right;}a{ text-decoration: none; font-size: 12px; margin: 0px; }a:hover{ color: #ffffff;}.but:hover{ background-color:#ffffff; color: black; }</style> <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css' integrity='sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z' crossorigin='anonymous'> <script src='https://code.jquery.com/jquery-3.5.1.slim.min.js' integrity='sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj' crossorigin='anonymous'></script> <script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js' integrity='sha384-9/reFTGAW83EW2RDu2S0VKaIzap3H66lZH81PoYlFhbGU+6BZp6G7niu735Sk7lN' crossorigin='anonymous'></script> <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js' integrity='sha384-B4gt1jrGC7Jh4AgTPSdUtOBvfO8shuf57BaghqFfPlYxofvL8/KUEfYiJOMMV+rV' crossorigin='anonymous'></script> <link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH' crossorigin='anonymous'> <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css'></head><body> <div class='outer-container' id = 'sectionLogin'> <div class='form'> <form class='forms'> <div class = 'make-flex'> <h1 class='head'>Welcome</h1> <p class='para'>Please Enter your details</p> <input type='text' placeholder='Enter your username' class='user m-2'/> <input type='password' placeholder='Enter password' class='pass m-2'/> <div class='anchors'> <a href=''>Forgot Password</a> </div> <button class='but' onclick = 'check_password()' href=''>Sign in</button> </div> </form> </div> </div> <div id = 'sectionLogged'> <div class='d-none IP'>" + WiFi.localIP().toString() + "</div> <div class='out-container'> <div class = 'containers shadow'> <div class='place d-flex justify-content-between '> <div> <h1 class='font-size-place'>TAMIL NADU</h1> <h1 class='font-size-change'>INDIA</h1> </div> <div class='time'> <h1 class='font-size-change'><span class='date font-size-change'></span> <span class='font-size-change month'></span></h1> <h1 class='font-size-change day'></h1> </div> </div> <div class='hr'></div> <div class='temperature d-flex justify-content-evenly'> <h1 class ='temp'><span class='convert-temp'>" + appendtemp + "</span> °</span></h1> <h1 class='temp'><i class='fa-solid fa-temperature-half'></i></h1> </div> <div class='hr'></div> <div class='d-flex justify-content-around'> <div class = 'humidity d-flex w-25 justify-content-around m-3'> <h3 class='humi-icon mt-auto mb-auto'><img width='40' height='40' src='https://res.cloudinary.com/dredwklnr/image/upload/v1713094637/humidity_sj7wxt.png' alt='humidity'/></h3> <h3 class='mt-auto mb-auto '>" + appendhumi + "%</h3> </div> <div class = 'd-flex w-25 justify-content-around m-3'> <h3 class='mt-auto mb-auto '> <img width='40' height='40' src='https://res.cloudinary.com/dredwklnr/image/upload/v1713095105/fan_nb7r1h.png'/> </h3> <h3 class='mt-auto mb-auto fan'>75%</h3> </div> <div class='change-f-to-c d-flex w-25 justify-content-around m-3'> <h3 class='mt-auto mb-auto celsious' onclick='convertTocelsious()'>C°</h3> <h3 class='mt-auto mb-auto text-second farenheit' onclick = 'convertToFaren()'>F°</h3> </div> </div> <div class='hr mt-3'></div> <div class='custom-control custom-switch d-flex justify-content-center mt-3 '> <input type='checkbox' class='custom-control-input check-it' id='customSwitches' onchange='checkIt()' checked> <label class='custom-control-label check' for='customSwitches'>Turn on/off Fan</label> </div> </div> </div> </div> <script src='https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js' integrity='sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz' crossorigin='anonymous'></script> <script src='https://kit.fontawesome.com/a7ba78d494.js' crossorigin='anonymous'></script> <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js'></script> <script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js'></script> <script> let buttonHref = document.querySelector('.but'); const form = document.querySelector('form'); let loginPage = document.getElementById('sectionLogin'); let hidepage = document.getElementById('sectionLogged'); let errorElement = document.createElement('p'); let containerElement = document.querySelector('.make-flex'); errorElement.innerHTML = 'Incorrect Password or Username'; errorElement.style.fontStyle ='italic'; errorElement.style.color = 'red'; errorElement.style.fontWeight = 'bold'; form.addEventListener('submit', function(event) { event.preventDefault(); }); hidepage.style.display = 'none'; function check_password() { let username = document.querySelector('.user').value; let password = document.querySelector('.pass').value; if (password == 'Saran90@' && username == 'saranvairaperumal2003@gmail.com'){ errorElement.remove(); console.log('Login Successfull!!!!'); loginPage.style.display = 'none'; hidepage.style.display = 'block'; hidepage.style.visibility = 'visible'; } else{ containerElement.appendChild(errorElement); console.log('Incorrect Username or Password'); } } </script><script> let fan = document.querySelector('.fan'); let check = document.querySelector('.check'); let date = new Date(); let set_date = document.querySelector('.date'); set_date.innerHTML = date.getDate(); let days = ['SUNDAY' , 'MONDAY' , 'TUESDAY' , 'WEDNESDAY' ,'THURSDAY' , 'FRIDAY' , 'SATURDAY']; let months = ['JANUARY', 'FEBRUARY', 'MARCH', 'APRIL', 'MAY', 'JUNE', 'JULY', 'AUGUST', 'SEPTEMBER', 'OCTOBER', 'NOVEMBER', 'DECEMBER']; document.querySelector('.day').innerHTML = days[date.getDay()]; document.querySelector('.month').innerHTML = months[date.getMonth()]; function convertToFaren(){ let findcolor = document.querySelector('.text-second').style.color; let celsius = document.querySelector('.convert-temp'); celsius.innerHTML = `${Math.floor(parseInt(celsius.innerHTML) * 9/5 + 32)}`; } function convertTocelsious(){ let findcolor = document.querySelector('.celsious').style.color; let faren = document.querySelector('.convert-temp'); faren.innerHTML = `${Math.floor((parseInt(faren.innerHTML) - 32 ) * 5/9 )}`; } function checkIt(){ let selectChecked = document.querySelector('.check-it'); let IP = document.querySelector('.IP').innerHTML; let serverAddress = `http://${IP}/`; let request = new XMLHttpRequest(); if(selectChecked.checked){ serverAddress += 'turnOn'; request.open('GET' , serverAddress ,true); request.onload = function(){ if(request.status == 200){ console.log('ok'); fan.innerHTML = '75%'; check.innerHTML ='ON' } else{ console.log('not ok') } }; request.send(); console.log('true') } else{ serverAddress += 'turnOff'; request.open('GET' , serverAddress ,true); request.onload = function(){ if(request.status == 200){ console.log('ok'); fan.innerHTML = '00%'; check.innerHTML ='OFF' } else{ console.log('not ok') } }; request.send(); console.log('false') } }</script></body></html>";
  server.send(200 , "text/html" , login_html);
}
void turn_off(){
   fan_state = 1;
   Serial2.print(0);
   server.send(200);
}

void turn_on(){
  fan_state =0;
  Serial2.print(1);
  server.send(200);
}