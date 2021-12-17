<!-- the Index File of angorith4.org -->
<!-- Designed by Angold Wang Dec 2021 -->

<!DOCTYPE html>
<html><head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<link href="css/index.css" rel="stylesheet">
<title>Angorithm4 Home Page</title>
<link href="photo/logo.png" rel="icon">
<!-- path= -->
</head>
<body>
<div class=nosearch>
<a href="index.html">
<img class="logo" src="photo/logo_dk.png" alt="A4" border="0" width="101" height="101">


<img class="logo" src="photo/mmm.jpeg" alt="mmm" border="0" width="250" height="104">
</a>
<div><!-- IE hack to prevent disappearing logo --></div>
<div class="tagline desktoponly">
Interpret!<br> The World.
</div>
<div class="menu mainmenu">
<ul>
<li><a href="index.html">Home</a>
<li class='mobileonly'><a href="javascript:void(0)" onclick='toggle_div("submenu")'>Menu</a>
<li class='wideonly'><a href='about.html'>About</a>
<li class='desktoponly'><a href="docs.html">Documentation</a>
<li class='desktoponly'><a href="carbon.php">Carbon</a>
<li class='desktoponly'><a href="llss.html">LLSS</a>
<li class='wideonly'><a href='copyright.html'>Licence</a>
<li class='desktoponly'><a href="support.html">Support</a>
<li class='search' id='search_menubutton'>
<a href="javascript:void(0)" onclick='toggle_search()'>Search</a>
</ul>
</div>
<div class="menu submenu" id="submenu">
<ul>
<li><a href='about.html'>About</a>
<li><a href='carbon.php'>Carbon</a>
<li><a href='llss.html'>LLSS</a>
<li><a href='support.html'>Support</a>
</ul>
</div>
<div class="searchmenu" id="searchmenu">
<form method="GET" action="search">
<select name="s" id="searchtype">
<option value="d">Search Documentation</option>
<option value="c">Search Changelog</option>
</select>
<input type="text" name="q" id="searchbox" value="">
<input type="submit" value="Go">
</form>
</div>
</div>
<script>
function toggle_div(nm) {
var w = document.getElementById(nm);
if( w.style.display=="block" ){
w.style.display = "none";
}else{
w.style.display = "block";
}
}
function toggle_search() {
var w = document.getElementById("searchmenu");
if( w.style.display=="block" ){
w.style.display = "none";
} else {
w.style.display = "block";
setTimeout(function(){
document.getElementById("searchbox").focus()
}, 30);
}
}
function div_off(nm){document.getElementById(nm).style.display="none";}
window.onbeforeunload = function(e){div_off("submenu");}
/* Disable the Search feature if we are not operating from CGI, since */
/* Search is accomplished using CGI and will not work without it. */
if( !location.origin || !location.origin.match || !location.origin.match(/http/) ){
document.getElementById("search_menubutton").style.display = "none";
}
/* Used by the Hide/Show button beside syntax diagrams, to toggle the */
function hideorshow(btn,obj){
var x = document.getElementById(obj);
var b = document.getElementById(btn);
if( x.style.display!='none' ){
x.style.display = 'none';
b.innerHTML='show';
}else{
x.style.display = '';
b.innerHTML='hide';
}
return false;
}
</script>


<div>
  <?php
    $company = htmlspecialchars($_POST['company']);
    $start = htmlspecialchars($_POST['start']);
    $end = htmlspecialchars($_POST['end']);
    $offset = htmlspecialchars($_POST['offset']);
    $money = htmlspecialchars($_POST['money']);

    $ret = shell_exec("/var/www/wfw/src/carbon/indicator/stock/a.out 'sma_single_earn' $company $start $end $offset $money");

    echo "<pre>$ret</pre>"
  ?>
</div>





<br clear="both">
<hr class="xhr">
<p>Ongoing development and support of Angorithm4 is made possible in part
by these <a href = "people.html">members</a>, including:</p>
<a name="consortium_members"></a>
<style>
#sponsors {
  margin:auto;
  width:80%;
}
.onesponsor a img {
  width:100px;
  max-width:100px;
  padding:1ex;
}
</style>
<div id="sponsors"></div>
<script>
  var sponsors = [{
    "name":"Angold Wang",
    "href":"http://angorithm4.org/angoldw",
    "src":"photo/people/angold.jpeg",
  },{
    "name":"Shuo Feng",
    "href":"http://angorithm4.org/fds",
    "src":"photo/people/fds.jpeg",
  },{
    "name":"Seahole Tan",
    "href":"http://www.med-studios.com/",
    "src":"photo/people/seahole.jpeg",
  },{
    "name":"Yi Zhou",
    "href":"https://www.angorithm4.org/yi",
    "src":"photo/people/yi.jpeg",
  }];

  for(var i=0; i<sponsors.length; i++){sponsors[i].idx = i;}
  sponsors.sort(function(a,b){return a.idx-b.idx});
  var h = "";
  for (var i=0; i < sponsors.length; i++){
    h += "<span class='onesponsor'><a href='";
    h += sponsors[i].href;
    h += "'><img src='";
    h += sponsors[i].src;
    h += "'></a></span>\n";
  }
  document.getElementById("sponsors").innerHTML = h;

</script>
<br clear="both">
<hr class="xhr">

