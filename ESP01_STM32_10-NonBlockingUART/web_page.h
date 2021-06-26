#ifndef __WEB_PAGE_H
#define __WEB_PAGE_H
char *home ="<!DOCTYPE html>\n\
<html>\n\
<body>\n\
<h2>LED MATRIX DISPLAY INTERFACE</h2>\n\
<form action=\"/page1\">\n\
	<label for=\"fstring\">Type a String:</label>\n\
    <input type=\"text\" id=\"fstring\" name=\"fstring\" value=\"string\"></input>\n\
  <input type= \"submit\" value=\"SUBMIT\">\n\
</form>\n\
</body>\n\
</html>";

char *page1 ="<!DOCTYPE html>\n\
<html>\n\
<body>\n\
<h2>LED MATRIX DISPLAY INTERFACE</h2>\n\
<form action=\"/homepage\">\n\
  <input type= \"submit\" value=\"SUBMIT AGAIN!\">\n\
</form>\n\
</body>\n\
</html>";
#endif
