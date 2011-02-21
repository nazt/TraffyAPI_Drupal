ดองมาจะครบเดือนอยู่แล้ว ต้องอัพเดตเสียหน่อย
เกริ่น
--------------
blog ตอนที่แล้ว [ดูภาพจากกล้อง CCTV ทั่วกรุงเทพ ด้วย TraffyAPI (และ Drupal7)](http://www.together.in.th/view-cctv-camera-using-traffy-api-and-drupal7/)   ได้ทำ drupal7 module เพื่อทดสอบ Traffy API
ก็ได้พบกับปัญหาจุกจิกเล็กๆน้อย ปัญหาหลักๆที่กวนใจก็คือเรื่อง http referer เนี่ยแหละ
แต่จริงๆผมอาจจะติดกับพวก Public  Facebook API หรือ Twitter API มากเกินไปที่ไม่ต้องใช้ http referrer

ปัญหาที่พบ (กับตัวเอง)
--------------

หลังจากที่ลองเล่น  Traffy API มานิดนึง เพื่อหวังหาไอเดียทำอะไรซักอย่าง ก็เจอกับปัญหา referrer เข้าอย่างจัง
มีวันนึงผมจะเขียน google chrome extension แต่ว่าเจอปัญหา Request ไปขอข้อมูลอะไรไม่ได้เลย เพราะมึนเรื่อง http referrer เนี่ยแหละ

ปัญหาที่ทำให้หมดแรงในการพัฒนา เพราะ มึนส์
--------------
> 1. หลักการตั้งชื่อไม่เหมือนกัน มีหลายแบบมากดังข้างล่าง
>        * getLinkALLInfo
>        * getCCTVimg
>        * getCCTV
> 2. api ในหัวเอกสารไม่ตรงกับที่ต้องเรียกจริงๆ
>        * เอกสาร: getTrafficCongestion แต่จริงๆแล้วต้องเรียก getCL
> [ดูหัวเอกสารที่ info.traffy](http://info.traffy.in.th/gettrafficcongestion/)
>        * เอกสาร: getCCTVimg แต่จริงๆ ต้องเรียก getcctvimg
> [ดูหัวเอกสารที่ info.traffy](http://info.traffy.in.th/getcctvimg/)
> 3. case sensitive ที่ชื่อ api และ format ถ้าไม่ใช่ XML/JSON/TABLE มันเจ๊งงง
> 4. output จาก api มีไม่เหมือนกันเลย เช่นบางอัน มี JSON บางอันไม่มี เป็นต้น

สิ่งที่ทำ
--------------

ผมเลยทำ Traffy API Wrapper ซะเลย แต่ไม่รู้ว่าจะเรียกว่า Wrapper หรือ  Proxy ดี แต่ชอบคำว่า Wrapper  มากกว่า มันดูเท่ดี (ว่าไปนั่น)

หน้าที่ของสิ่งที่ทำ
--------------
หน้าที่ของ module Traffy API Wrapper มีหน้าที่แค่ รับ request จากทางบ้าน แล้ว request ต่อไปที่ Server Traffy API

    อ้าว!! แล้วมันแตกต่างกับการเรียกใช้ Traffy API ของ Nectec ยังไงล่ะ?

ความแตกต่างระหว่าง Traffy API กับ Traffy API Wrapper
--------------
1. สามารถใช้ได้เลย ไม่ต้องใช้ apikey และ appid (เฉพาะ api ที่ผมคิดว่า ไม่จำเป็นต้อง authenticate)
2. (อาจจะ) ไม่ต้องปวดหัวกับ ปัญหา referrer
3. map api ให้ตรงกับหัวเอกสาร (เช่น 'getTrafficCongestion' =&gt; 'getCL', 'getCCTVimg' =&gt; 'getcctvimg') แก้ไขปัญหาหัวเอกสารไม่ตรงกับ api
4. ปรับให้ใช้เรียก api โดยไม่สนใจอักขระตัวเล็กตัวใหญ่ (case insensitive) ทำให้สามารถเรียก getcctv, getcctvimg, gettrafficcongestion ได้เลย
5. cache traffy api  ดังนี้
    cache api เป็นเวลา 5 นาที ดังนี้
    'getcctv' => array('cid' => 'traffy_'.$api_to_call, 'type' => 'cache_page', 'expire' => 5*60),
    'getcctvimg' => array('cid' => 'traffy_'.$api_to_call, 'type' => 'cache_page', 'expire' => 5*60),
    );

สำหรับแนวทางการพัฒนาต่อไป
--------------
- ผมมีแผนที่จะแคชข้อมูลบางตัวให้ เพื่อแก้ไขปัญหาความช้าของ Server ที่ส่งผลลัพธ์
- ลืมแล้วว่าคิดอะไรไว้ !!

วิธีการใช้งาน
--------------
1. เลือก api ที่ต้องการใช้งานจาก [Traffy Info](http://info.traffy.in.th/api/) แต่แนะนำให้ดู API จากเมนูด้านบน ข้างๆเมนู "หน้าแรก" หรือ ตาม ข้างล่าง
```
    $map = array( 'getcctvimg' => 'getcctvimg',
                  'getcctv' => 'getCCTV',
                  'getvms' => 'getVMS',
                  'getvmsimg' => 'getvmsimg',
                  'getincident', => 'getIncident',
                  'getlinkallinfo' => 'getLinkALLInfo',
                  'getrainforecast' => 'getRainForecast',
                  'gettrafficcongestion' => 'getCL',
                  'getlinkinfo' => 'getLinkInfo',
                  'gettraveltime' => 'getTravelTime',
                   //postAPI
                  'postgpsdata' => 'postGPSData',
                  'postincident' => 'postIncident',
                );
```

สิ่งที่ผมไม่ได้ทำ
-------------

1.  ผมไม่ได้ handle เวลาที่ api error

2. request api ไปที่ path http://www.together.in.th/drupal/traffy/wrapper/(api-to-call)  ตัวอย่างเช่น

>   - [http://www.together.in.th/drupal/traffy/wrapper/getcctv](http://www.together.in.th/drupal/traffy/wrapper/getcctv)
>   - [http://www.together.in.th/drupal/traffy/wrapper/getcctvimg?id=255](http://www.together.in.th/drupal/traffy/wrapper/getcctvimg?id=255)

>   หรือ request แบบพิเศษได้ประมาณนี้ http://www.together.in.th/drupal/traffy/wrapper/getcctv/[HEADER]

>   - [http://www.together.in.th/drupal/traffy/wrapper/getcctv/json](http://www.together.in.th/drupal/traffy/wrapper/getcctv/JSON)
>   - [http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/jpeg/?id=255](http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/jpeg/?id=255)

>   หรือ reques แบบส่ง [HEADER] ไปทาง query string ($_GET['header']) ก็ได้  อย่างเช่น
>   - [http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/?id=255&header=jpeg](http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/?id=25&header=jpeg)

รายชื่อ header ที่ทั้งหมดที่ใช้ได้
```
    $map = array(
                  'img' => 'image/jpeg',
                  'image' => 'image/jpeg',
                  'png' => 'image/png',
                  'jpeg' => 'image/jpeg',
                  'html' => 'text/html',
                  'xml' => 'text/xml',
                  'javascript' => 'application/javascript',
                  'js' => 'application/javascript',
                  'json' => 'application/json',
                );
```
api ที่มีปัญหา
-------
```
    //bad
    gettrafficcongestion มีปัญหาเพราะ parameter $_GET['q'] มันซ้ำกัน
    getlinkallinfo  ต้องระบุ format = csv/table
```

สิ่งที่ผมไม่ได้ทำ
-------
* ไม่ handle เหตุการณ์เมื่อมี error



ลองใช้งาน
-------
* [http://www.together.in.th/drupal/traffy/wrapper/getcctv](http://www.together.in.th/drupal/traffy/wrapper/getcctv)
* [http://www.together.in.th/drupal/traffy/wrapper/getcctvimg?id=255](http://www.together.in.th/drupal/traffy/wrapper/getcctvimg?id=255)

    > request แบบพิเศษ http://www.together.in.th/drupal/traffy/wrapper/getcctv/[HEADER]

* [http://www.together.in.th/drupal/traffy/wrapper/getcctv/json](http://www.together.in.th/drupal/traffy/wrapper/getcctv/json)
* [http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/JPEG/?id=255](http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/JPEG/?id=255)

    > request แบบส่ง [HEADER] ไปทาง query string ($_GET['header'])
* [http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/?id=255&header=jpeg](http://www.together.in.th/drupal/traffy/wrapper/getcctvimg/?id=25&header=jpeg)

    > อื่นๆ
* [http://www.together.in.th/drupal/traffy/wrapper/getlinkallinfo?format=csv](http://www.together.in.th/drupal/traffy/wrapper/getlinkallinfo?format=csv)
* [http://www.together.in.th/drupal/traffy/wrapper/getvms?format=json&header=json](http://www.together.in.th/drupal/traffy/wrapper/getvms?format=json&header=json)
* [http://www.together.in.th/drupal/traffy/wrapper/getvmsimg/png/?id=3](http://www.together.in.th/drupal/traffy/wrapper/getvmsimg/png/?id=3)
* [http://www.together.in.th/drupal/traffy/wrapper/getvmsimg/?id=3&header=png](http://www.together.in.th/drupal/traffy/wrapper/getvmsimg/?id=3&header=png)