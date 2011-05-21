<?php
function test_api($api_to_call) {
  $traffy  = get_api_info();
  $link_opts = array_merge(array('api' => $api_to_call), get_link_option());

  if (!isset($_GET['format'])) {
    $_GET['format'] = "JSON";
  }

  foreach ($_GET as $key => $value) {
    if ($key != 'q') {
      $link_opts[$key] = $value;
    }
  }

  $url = url($traffy['service'], array('query' => $link_opts));
  $options = array(
    'headers' => get_http_headers(),
    'method' => 'POST',
    'data' => NULL,
    'max_redirects' => 3,
    'timeout' => 10.0,
    'context' => NULL,
  );

    $ret = '';
    $res = drupal_http_request($url, $options);
    if ($res->code != 200 ) {
      drupal_set_message(print_r($res, 1), 'error');
      drupal_set_message($res->error, 'error');
      $ret = 'error '. $res->code .': ' . $res->error;
    }
    else {
      switch ($api_to_call) {
        case "getCCTV":
          $raw_json = $res->data;
          /**
            *  I CAN'T PARSE JSON !!!!
            *  Return javascript code,,, : (((
            *
            */
          $ret = sprintf("<a href='%s'>%s</a>", "http://www.together.in.th/see-all-cctv-camera-bangkok-traffy-wrapper-api-jquery-json/", "ดูกล้อง CCTV ทั่วกรุงเทพ ด้วย Traffy API [Wrapper/Proxy] + jQuery + JSONP Version ใหม่");
          $ret .= "<script>
            jQuery(document).ready(function() {
              jQuery('#content').append('<div id=\'traffy\'> <div class=\'loading\'>Loading</div> </div>');
              var c = ". $raw_json ."
              jQuery.each(c[0], function(k, v) {
                v.url = '/drupal/traffy/wrapper/getcctvimg?id='+k;
                var img_ele = jQuery('<img />'); img_ele.attr('src', v.url);
                jQuery('#traffy').append(img_ele);
                if(typeof(console) !== 'undefined' && console != null) {
                  console.log(k, v);
                }
              });
              jQuery('.loading').remove();
            });
          </script>";
          break;
        case "getcctvimg":
            drupal_add_http_header('Content-Type', 'image/jpeg');
            print $res->data;
            exit;
          break;
        default;
            $ret = 'api not found';
      }
    }
  return $ret;
}

