<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.9" tiledversion="1.9.2" name="Enemies" tilewidth="8" tileheight="8" tilecount="448" columns="28">
 <image source="Enemies.png" width="224" height="128"/>
 <tile id="0">
  <animation>
   <frame tileid="312" duration="150"/>
   <frame tileid="313" duration="150"/>
   <frame tileid="314" duration="150"/>
   <frame tileid="315" duration="150"/>
   <frame tileid="316" duration="150"/>
   <frame tileid="317" duration="150"/>
  </animation>
 </tile>
 <tile id="60">
  <animation>
   <frame tileid="60" duration="150"/>
   <frame tileid="61" duration="150"/>
   <frame tileid="62" duration="150"/>
   <frame tileid="63" duration="150"/>
  </animation>
 </tile>
 <tile id="88">
  <animation>
   <frame tileid="88" duration="150"/>
   <frame tileid="89" duration="150"/>
   <frame tileid="90" duration="150"/>
   <frame tileid="91" duration="150"/>
   <frame tileid="90" duration="150"/>
   <frame tileid="89" duration="150"/>
  </animation>
 </tile>
 <tile id="116">
  <animation>
   <frame tileid="116" duration="150"/>
   <frame tileid="117" duration="150"/>
   <frame tileid="118" duration="150"/>
   <frame tileid="119" duration="150"/>
  </animation>
 </tile>
 <tile id="140">
  <objectgroup draworder="index" id="2">
   <object id="1" x="3.42394" y="1.20301" width="3.5936" height="6.7862"/>
  </objectgroup>
  <animation>
   <frame tileid="140" duration="150"/>
   <frame tileid="141" duration="150"/>
   <frame tileid="142" duration="150"/>
  </animation>
 </tile>
 <tile id="141">
  <objectgroup draworder="index" id="2">
   <object id="1" x="2.96125" y="0.97166" width="4.02545" height="7.00212"/>
  </objectgroup>
 </tile>
 <tile id="142">
  <objectgroup draworder="index" id="2">
   <object id="1" x="3.00752" y="1.07962" width="3.96376" height="6.87874"/>
  </objectgroup>
 </tile>
 <tile id="308">
  <objectgroup draworder="index" id="2">
   <object id="1" x="3.97918" y="0" width="4.05629" height="4.99711"/>
  </objectgroup>
 </tile>
 <tile id="312">
  <objectgroup draworder="index" id="2">
   <object id="1" x="4.01003" y="0.0771159" width="3.96376" height="4.91999"/>
  </objectgroup>
  <animation>
   <frame tileid="312" duration="150"/>
   <frame tileid="313" duration="150"/>
   <frame tileid="314" duration="150"/>
   <frame tileid="315" duration="150"/>
   <frame tileid="316" duration="150"/>
   <frame tileid="317" duration="150"/>
  </animation>
 </tile>
 <tile id="313">
  <objectgroup draworder="index" id="2">
   <object id="1" x="4.02545" y="0.0154232" width="3.88664" height="4.98168"/>
  </objectgroup>
 </tile>
 <tile id="314">
  <objectgroup draworder="index" id="2">
   <object id="1" x="4.04087" y="0.0308463" width="3.93291" height="4.95084"/>
  </objectgroup>
 </tile>
 <tile id="315">
  <objectgroup draworder="index" id="2">
   <object id="1" x="4.01003" y="-0.0154232" width="3.97918" height="4.99711"/>
  </objectgroup>
 </tile>
 <tile id="316">
  <objectgroup draworder="index" id="2">
   <object id="1" x="3.9946" y="-0.138809" width="4.22595" height="5.08965"/>
  </objectgroup>
 </tile>
 <tile id="317">
  <objectgroup draworder="index" id="3">
   <object id="5" x="4.01003" y="0" width="3.97918" height="4.98168"/>
  </objectgroup>
 </tile>
 <tile id="340">
  <animation>
   <frame tileid="340" duration="150"/>
   <frame tileid="341" duration="150"/>
   <frame tileid="342" duration="150"/>
   <frame tileid="343" duration="150"/>
  </animation>
 </tile>
 <tile id="396">
  <animation>
   <frame tileid="396" duration="150"/>
   <frame tileid="397" duration="150"/>
   <frame tileid="398" duration="150"/>
  </animation>
 </tile>
 <wangsets>
  <wangset name="Unbenannter Satz" type="mixed" tile="-1">
   <wangcolor name="Wall" color="#ff0000" tile="-1" probability="1"/>
   <wangcolor name="Air" color="#00ff00" tile="-1" probability="1"/>
   <wangtile tileid="352" wangid="1,1,1,1,1,1,1,1"/>
   <wangtile tileid="381" wangid="2,2,2,2,2,2,2,2"/>
  </wangset>
 </wangsets>
</tileset>
