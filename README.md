#Expected Summary Code:
Should go to Host: ecse3038-lab5-tester.onrender.com on route message and print out the 2 lines on an lcd using an esp32 using a get request and api key

##void_setup 
sets baud rate and initialises lcd and connects device to internet

##void_loop
continuously performs the http post request on the route /temp first  the temp value is first read on an adc port and then serialized to a json value for the web page to process

##void httpget()
performs the get request in route/light and turn the light one

Both functions were taught previously in this lab the esp32 was both use to send and receive data using the internet where it bridged communications between a real cicruit and webpage.
