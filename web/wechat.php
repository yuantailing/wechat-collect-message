<?php
require_once("settings.php");
require_once("includes/database.php");
require_once("message.function.php");
include_once("wxBizMsgCrypt.php");
/**
  * wechat php
  */

//define your token

header('Content-type: text/plain; charset=UTF-8');
$wechatObj = new WechatCallbackApi();
$conn = newDBConnection($database_info);
if (array_key_exists("echostr", $_GET)) {
	$wechatObj->valid();
} else {	
	$wechatObj->responseMsg($conn);
}
$conn->close();

class WechatCallbackApi
{
	public function valid()
    {
        $echoStr = $_GET["echostr"];

        //valid signature, option
        if($this->checkSignature()){
        	echo $echoStr;
        	exit;
        }
    }

    public function responseMsg($conn)
    {
        if (!$this->checkSignature())
			return;

		//get post data, May be due to the different environments
		$postStr = $GLOBALS["HTTP_RAW_POST_DATA"];
		$token = TOKEN;
		$encodingAesKey = ENCODING_AES_KEY;
		$appId = APP_ID;
		$pc = new WXBizMsgCrypt($token, $encodingAesKey, $appId);
		$timeStamp = $_GET["timestamp"];
		$nonce = $_GET["nonce"];
		$msg_sign = $_GET["msg_signature"];
		$errCode = $pc->decryptMsg($msg_sign, $timeStamp, $nonce, $postStr, $msg);
		// $msg = $postStr; $errCode = 0; // 明文模式

      	//extract post data
		if ($errCode == 0) {
                /* libxml_disable_entity_loader is to prevent XML eXternal Entity Injection,
                   the best way is to check the validity of xml by yourself */   
                libxml_disable_entity_loader(true);
              	$msg = simplexml_load_string($msg, 'SimpleXMLElement', LIBXML_NOCDATA);
				$to_user_name = $msg->ToUserName;
				$from_user_name = $msg->FromUserName;
				$create_time = $msg->CreateTime;
				$msg_type = $msg->MsgType;
				$content = $msg->Content;
				$pic_url = $msg->PicUrl;
				$media_id = $msg->MediaId;
				$thumb_media_id = $msg->ThumbMediaId;
				$format = $msg->Format;
				$recognition = $msg->Recognition;
				$location_x = $msg->Location_X;
				$location_y = $msg->Location_Y;
				$scale = $msg->Scale;
				$label = $msg->Label;
				$title = $msg->Title;
				$description = $msg->Description;
				$url = $msg->Url;
				$msg_id = $msg->MsgId;
				$ret = message_insert_text_message($conn, $to_user_name, $from_user_name, $create_time, $msg_type, $content, $pic_url, $media_id, $thumb_media_id,
					$format, $recognition, $location_x, $location_y, $scale, $label, $title, $description, $url, $msg_id);
				if ($ret["error"] === 0) {
					if (preg_match('/^[Dd][Mm]([\s\S]*\S[\s\S]*)/i', $content, $matches)) {
						$ret = message_insert_danmu($conn, $ret["message_id"], $matches[1]);
						if ($ret["error"] === 0)
							$reply_content = "3！2！1！弹幕发射~升空！";
						else
							$reply_content = "Sorry, server error.";
					} else {
						$reply_content = "你好！由于微信公众平台只能回复两日内的消息，有可能我们的人员看到您的消息时已经没有办法回复了，" .
							"所以若有什么问题最好到课程的讨论区中反馈。目前我们在学堂在线(www.xuetangx.com)、edX(www.edx.org，适用于国外用户)" .
							"各有一个课堂，名为“数据结构”，欢迎大家注册。";
					}
				} else {
					$reply_content = "Sorry, server error.";
				}
				$template = "<xml>" .
					"<ToUserName><![CDATA[%s]]></ToUserName>" .
					"<FromUserName><![CDATA[%s]]></FromUserName>" .
					"<CreateTime>%d</CreateTime>" .
					"<MsgType><![CDATA[%s]]></MsgType>" .
					"<Content><![CDATA[%s]]></Content>" .
					"</xml>";
				$reply = sprintf($template, $from_user_name, $to_user_name, time(), "text", $reply_content);
				$pc->encryptMsg($reply, $timeStamp, $nonce, $encrypt);
				// $encrypt = $reply; // 明文模式
				print($encrypt);
        }
    }

	private function checkSignature()
	{
        // you must define TOKEN by yourself
        if (!defined("TOKEN")) {
            throw new Exception('TOKEN is not defined!');
        }

        $signature = $_GET["signature"];
        $timestamp = $_GET["timestamp"];
        $nonce = $_GET["nonce"];

		$token = TOKEN;
		$tmpArr = array($token, $timestamp, $nonce);
        // use SORT_STRING rule
		sort($tmpArr, SORT_STRING);
		$tmpStr = implode($tmpArr);
		$tmpStr = sha1($tmpStr);

		if ($tmpStr == $signature) {
			return true;
		} else {
			return false;
		}
	}
}

?>