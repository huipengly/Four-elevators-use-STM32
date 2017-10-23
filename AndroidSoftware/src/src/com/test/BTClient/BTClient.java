package com.test.BTClient;



import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import com.test.BTClient.DeviceListActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
//import android.view.Menu;            //如使用菜单加入此三包
//import android.view.MenuInflater;
//import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import android.graphics.Color;  

public class BTClient extends Activity {
	
	private final static int REQUEST_CONNECT_DEVICE = 1;    //宏定义查询设备句柄
	
	private final static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";   //SPP服务UUID号
	
	private InputStream is;    //输入流，用来接收蓝牙数据
	//private TextView text0;    //提示栏解句柄
    private EditText edit0;    //发送数据输入句柄
    private TextView dis;       //接收数据显示句柄
    private ScrollView sv;      //翻页句柄
    private String smsg = "";    //显示用数据缓存
    private String statemsg = "";    //显示用数据缓存
    private String fmsg = "";    //保存用数据缓存
    
    private Button runF;
    private Button stopF;
    private Button l1u;
    private Button l2u;
    private Button l2d;
    private Button l3u;
    private Button l3d;
    private Button l4d;
    private Button l1;
    private Button l2;
    private Button l3;
    private Button l4;
    private Button opendoor;
    private Button rdyclose;
    private Button closedoor;
    private Button rdyopen;
    private Button up;
    private Button down;
    private Button wait;
    private TextView lift;
    private TextView Text0;
    

    public String filename=""; //用来保存存储的文件名
    BluetoothDevice _device = null;     //蓝牙设备
    BluetoothSocket _socket = null;      //蓝牙通信socket
    boolean _discoveryFinished = false;    
    boolean bRun = true;
    boolean bThread = false;
	
    private BluetoothAdapter _bluetooth = BluetoothAdapter.getDefaultAdapter();    //获取本地蓝牙适配器，即蓝牙设备
	
	
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);   //设置画面为主画面 main.xml
        
        //text0 = (TextView)findViewById(R.id.Text0);  //得到提示栏句柄
//        edit0 = (EditText)findViewById(R.id.Edit0);   //得到输入框句柄
//        sv = (ScrollView)findViewById(R.id.ScrollView01);  //得到翻页句柄
//        dis = (TextView) findViewById(R.id.in);      //得到数据显示句柄

        lift = (TextView) findViewById(R.id.textView1);
        runF = (Button) findViewById(R.id.button9);
        stopF = (Button) findViewById(R.id.button7);
        l1u = (Button) findViewById(R.id.button6);
        l2u = (Button) findViewById(R.id.button5);
        l2d = (Button) findViewById(R.id.button4);
        l3u = (Button) findViewById(R.id.button3);
        l3d = (Button) findViewById(R.id.button2);
        l4d = (Button) findViewById(R.id.button1);
        l1 = (Button) findViewById(R.id.button12);
        l2 = (Button) findViewById(R.id.button13);
        l3 = (Button) findViewById(R.id.button14);
        l4 = (Button) findViewById(R.id.button15);
        opendoor = (Button) findViewById(R.id.button17);
        rdyclose = (Button) findViewById(R.id.button18);
        closedoor = (Button) findViewById(R.id.button16);
        rdyopen = (Button) findViewById(R.id.button19);
        up = (Button) findViewById(R.id.button10);
        down = (Button) findViewById(R.id.button11);
        wait = (Button) findViewById(R.id.button8);
        Text0 = (TextView)findViewById(R.id.Text0);
        
        runF.setBackgroundColor(Color.BLACK);
        stopF.setBackgroundColor(Color.BLACK);
        l1u.setBackgroundColor(Color.BLACK);
        l2u.setBackgroundColor(Color.BLACK);
        l2d.setBackgroundColor(Color.BLACK);
        l3u.setBackgroundColor(Color.BLACK);
        l3d.setBackgroundColor(Color.BLACK);
        l4d.setBackgroundColor(Color.BLACK);
        l1.setBackgroundColor(Color.BLACK);
        l2.setBackgroundColor(Color.BLACK);
        l3.setBackgroundColor(Color.BLACK);
        l4.setBackgroundColor(Color.BLACK);
        opendoor.setBackgroundColor(Color.BLACK);
        rdyclose.setBackgroundColor(Color.BLACK);
        closedoor.setBackgroundColor(Color.BLACK);
        rdyopen.setBackgroundColor(Color.BLACK);
        up.setBackgroundColor(Color.BLACK);
        down.setBackgroundColor(Color.BLACK);
        wait.setBackgroundColor(Color.BLACK);
        

       //如果打开本地蓝牙设备不成功，提示信息，结束程序
        if (_bluetooth == null){
        	Toast.makeText(this, "无法打开手机蓝牙，请确认手机是否有蓝牙功能！", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        
        // 设置设备可以被搜索  
       new Thread(){
    	   public void run(){
    		   if(_bluetooth.isEnabled()==false){
        		_bluetooth.enable();
    		   }
    	   }   	   
       }.start();      
    }

    //发送按键响应
/*    public void onSendButtonClicked(View v){
    	int i=0;
    	int n=0;
    	try{
    		OutputStream os = _socket.getOutputStream();   //蓝牙连接输出流
    		byte[] bos = edit0.getText().toString().getBytes();
    		for(i=0;i<bos.length;i++){
    			if(bos[i]==0x0a)n++;
    		}
    		byte[] bos_new = new byte[bos.length+n];
    		n=0;
    		for(i=0;i<bos.length;i++){ //手机中换行为0a,将其改为0d 0a后再发送
    			if(bos[i]==0x0a){
    				bos_new[n]=0x0d;
    				n++;
    				bos_new[n]=0x0a;
    			}else{
    				bos_new[n]=bos[i];
    			}
    			n++;
    		}
    		
    		os.write(bos_new);	
    	}catch(IOException e){  		
    	}  	
    }*/
    
    //接收活动结果，响应startActivityForResult()
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
    	switch(requestCode){
    	case REQUEST_CONNECT_DEVICE:     //连接结果，由DeviceListActivity设置返回
    		// 响应返回结果
            if (resultCode == Activity.RESULT_OK) {   //连接成功，由DeviceListActivity设置返回
                // MAC地址，由DeviceListActivity设置返回
                String address = data.getExtras()
                                     .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                // 得到蓝牙设备句柄      
                _device = _bluetooth.getRemoteDevice(address);
 
                // 用服务号得到socket
                try{
                	_socket = _device.createRfcommSocketToServiceRecord(UUID.fromString(MY_UUID));
                }catch(IOException e){
                	Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                }
                //连接socket
            	Button btn = (Button) findViewById(R.id.Button03);
                try{
                	_socket.connect();
                	Toast.makeText(this, "连接"+_device.getName()+"成功！", Toast.LENGTH_SHORT).show();
                	btn.setText("断开");
                }catch(IOException e){
                	try{
                		Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                		_socket.close();
                		_socket = null;
                	}catch(IOException ee){
                		Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                	}
                	
                	return;
                }
                
                //打开接收线程
                try{
            		is = _socket.getInputStream();   //得到蓝牙数据输入流
            		}catch(IOException e){
            			Toast.makeText(this, "接收数据失败！", Toast.LENGTH_SHORT).show();
            			return;
            		}
            		if(bThread==false){
            			ReadThread.start();
            			bThread=true;
            		}else{
            			bRun = true;
            		}
            }
    		break;
    	default:break;
    	}
    }
    
    //接收数据线程
    Thread ReadThread=new Thread(){
    	
    	public void run(){
    		int num = 0;
    		byte[] buffer = new byte[1024];
    		byte[] buffer_new = new byte[1024];
    		int i = 0;
    		int n = 0;
    		bRun = true;
    		//接收线程
    		while(true){
    			try{
    				while(is.available()==0){
    					while(bRun == false){}
    				}
    				while(true){
    					num = is.read(buffer);         //读入数据
    					n=0;
    					
    					String s0 = new String(buffer,0,num);
    			//		fmsg+=s0;    //保存收到数据
    				/*	for(i=0;i<num;i++){
    						if((buffer[i] == 0x0d)&&(buffer[i+1]==0x0a)){
    							buffer_new[n] = 0x0a;
    							i++;
    						}else{
    							buffer_new[n] = buffer[i];
    						}
    						n++;
    					}
    					String s = new String(buffer_new,0,n);*/
    					smsg+=s0;   //写入接收缓存
    				//	smsg+='|';
    				//	smsg = s;
    					if(is.available()==0)break;  //短时间没有数据才跳出进行显示
    				}
    			//	if(smsg.length() == 22)
    				//发送显示消息，进行显示刷新
    					handler.sendMessage(handler.obtainMessage());       	    		
    	    		}catch(IOException e){
    	    		}
    		}
    	}
    };
    
    //数据处理
    private void dataHandle(){
//		button1.setBackgroundColor(Color.RED);
//    	smsg = "#10101010101010$";
    	if(smsg.length() > 21){

	//		Text0.setText(smsg);
    		if(smsg.substring(0,1).equals("#") && smsg.substring(21,22).equals("$")){
    			statemsg = smsg.substring(0, 22);//截取一帧
    			smsg = smsg.substring(22);//删除前一帧
    			    			
    			if(Integer.parseInt(statemsg.substring(1, 2))==1){
    		        runF.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        runF.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(2, 3))==1){
    		        stopF.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        stopF.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(3, 4))==1){
    		        l1u.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l1u.setBackgroundColor(Color.BLACK);
    			}
    			
    			if(Integer.parseInt(statemsg.substring(4, 5))==1){
    		        l2u.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l2u.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(5, 6))==1){
    		        l2d.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l2d.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(6, 7))==1){
    		        l3u.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l3u.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(7, 8))==1){
    		        l3d.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l3d.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(8, 9))==1){
    		        l4d.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l4d.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(9, 10))==1){
    		        l1.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l1.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(10, 11))==1){
    		        l2.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l2.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(11, 12))==1){
    		        l3.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l3.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(12, 13))==1){
    		        l4.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        l4.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(13, 14))==1){
    		        opendoor.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        opendoor.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(14, 15))==1){
    				rdyopen.setBackgroundColor(Color.BLUE);
    			}
    			else{
    				rdyopen.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(15, 16))==1){
    		        closedoor.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        closedoor.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(16, 17))==1){
    				rdyclose.setBackgroundColor(Color.BLUE);
    			}
    			else{
    				rdyclose.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(17, 18))==1){
    		        up.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        up.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(18, 19))==1){
    		        down.setBackgroundColor(Color.BLUE);
    			}
    			else{
    		        down.setBackgroundColor(Color.BLACK);
    			}

    			if(Integer.parseInt(statemsg.substring(19, 20))==1){
    		        wait.setBackgroundColor(Color.BLACK);
    			}
    			else{
    		        wait.setBackgroundColor(Color.BLUE);
    			}
    			
    			lift.setText(statemsg.substring(20, 21));
    		}
    		else{
    			if(smsg.substring(0, 1).equals("#")){
    				smsg = smsg.substring(1);
    			}
    			int size = smsg.indexOf("#");
    		//	lift.setText(String.valueOf(size));
    		//	Text0.setText(statemsg);
    			if(-1 != size){
    				smsg = smsg.substring(size);
    			}
    			else{
    				smsg = null;
    			}
    		}
    	}
    	
    		//if(smsg.substring(15,16) == "$"){

      //  		dis.setText(smsg);   //显示数据 
    		//}
    	//}
    //	else()
    	
    }
    
    //消息处理队列
    Handler handler= new Handler(){
    	public void handleMessage(Message msg){
    		super.handleMessage(msg);
    		//dis.setText(smsg);   //显示数据 
    		dataHandle();
    		//sv.scrollTo(0,dis.getMeasuredHeight()); //跳至数据最后一页
    	/*	if(0 == aa){
    			button1.setBackgroundColor(Color.RED);
    			aa = 1;
    		}
    		else{
    			button1.setBackgroundColor(Color.BLUE);
    			aa = 0;
    		}*/
    	}
    };
    
    //关闭程序掉用处理部分
    public void onDestroy(){
    	super.onDestroy();
    	if(_socket!=null)  //关闭连接socket
    	try{
    		_socket.close();
    	}catch(IOException e){}
    //	_bluetooth.disable();  //关闭蓝牙服务
    }
    
    //菜单处理部分
  /* @Override
    public boolean onCreateOptionsMenu(Menu menu) {//建立菜单
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        return true;
    }*/

  /*  @Override
    public boolean onOptionsItemSelected(MenuItem item) { //菜单响应函数
        switch (item.getItemId()) {
        case R.id.scan:
        	if(_bluetooth.isEnabled()==false){
        		Toast.makeText(this, "Open BT......", Toast.LENGTH_LONG).show();
        		return true;
        	}
            // Launch the DeviceListActivity to see devices and do scan
            Intent serverIntent = new Intent(this, DeviceListActivity.class);
            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
            return true;
        case R.id.quit:
            finish();
            return true;
        case R.id.clear:
        	smsg="";
        	ls.setText(smsg);
        	return true;
        case R.id.save:
        	Save();
        	return true;
        }
        return false;
    }*/
    
    //连接按键响应函数
    public void onConnectButtonClicked(View v){ 
    	if(_bluetooth.isEnabled()==false){  //如果蓝牙服务不可用则提示
    		Toast.makeText(this, " 打开蓝牙中...", Toast.LENGTH_LONG).show();
    		return;
    	}
    	
    	
        //如未连接设备则打开DeviceListActivity进行设备搜索
    	Button btn = (Button) findViewById(R.id.Button03);
    	if(_socket==null){
    		Intent serverIntent = new Intent(this, DeviceListActivity.class); //跳转程序设置
    		startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);  //设置返回宏定义
    	}
    	else{
    		 //关闭连接socket
    	    try{
    	    	
    	    	is.close();
    	    	_socket.close();
    	    	_socket = null;
    	    	bRun = false;
    	    	btn.setText("连接");
    	    	smsg = null;
    	    	statemsg = null;
    	        runF.setBackgroundColor(Color.BLACK);
    	        stopF.setBackgroundColor(Color.BLACK);
    	        l1u.setBackgroundColor(Color.BLACK);
    	        l2u.setBackgroundColor(Color.BLACK);
    	        l2d.setBackgroundColor(Color.BLACK);
    	        l3u.setBackgroundColor(Color.BLACK);
    	        l3d.setBackgroundColor(Color.BLACK);
    	        l4d.setBackgroundColor(Color.BLACK);
    	        l1.setBackgroundColor(Color.BLACK);
    	        l2.setBackgroundColor(Color.BLACK);
    	        l3.setBackgroundColor(Color.BLACK);
    	        l4.setBackgroundColor(Color.BLACK);
    	        opendoor.setBackgroundColor(Color.BLACK);
    	        rdyclose.setBackgroundColor(Color.BLACK);
    	        closedoor.setBackgroundColor(Color.BLACK);
    	        rdyopen.setBackgroundColor(Color.BLACK);
    	        up.setBackgroundColor(Color.BLACK);
    	        down.setBackgroundColor(Color.BLACK);
    	        wait.setBackgroundColor(Color.BLACK);
    	        lift.setText("0");
    	    }catch(IOException e){}   
    	}
    	return;
    }
    
    //保存按键响应函数
    public void onSaveButtonClicked(View v){
    	Save();
    }
    
    //清除按键响应函数
    public void onClearButtonClicked(View v){
    	smsg="";
    	fmsg="";
    	dis.setText(smsg);
    	return;
    }
    
    //退出按键响应函数
    public void onQuitButtonClicked(View v){
    	finish();
    }
    
    //保存功能实现
	private void Save() {
		//显示对话框输入文件名
		LayoutInflater factory = LayoutInflater.from(BTClient.this);  //图层模板生成器句柄
		final View DialogView =  factory.inflate(R.layout.sname, null);  //用sname.xml模板生成视图模板
		new AlertDialog.Builder(BTClient.this)
								.setTitle("文件名")
								.setView(DialogView)   //设置视图模板
								.setPositiveButton("确定",
								new DialogInterface.OnClickListener() //确定按键响应函数
								{
									public void onClick(DialogInterface dialog, int whichButton){
										EditText text1 = (EditText)DialogView.findViewById(R.id.sname);  //得到文件名输入框句柄
										filename = text1.getText().toString();  //得到文件名
										
										try{
											if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)){  //如果SD卡已准备好
												
												filename =filename+".txt";   //在文件名末尾加上.txt										
												File sdCardDir = Environment.getExternalStorageDirectory();  //得到SD卡根目录
												File BuildDir = new File(sdCardDir, "/data");   //打开data目录，如不存在则生成
												if(BuildDir.exists()==false)BuildDir.mkdirs();
												File saveFile =new File(BuildDir, filename);  //新建文件句柄，如已存在仍新建文档
												FileOutputStream stream = new FileOutputStream(saveFile);  //打开文件输入流
												stream.write(fmsg.getBytes());
												stream.close();
												Toast.makeText(BTClient.this, "存储成功！", Toast.LENGTH_SHORT).show();
											}else{
												Toast.makeText(BTClient.this, "没有存储卡！", Toast.LENGTH_LONG).show();
											}
										
										}catch(IOException e){
											return;
										}
										
										
										
									}
								})
								.setNegativeButton("取消",   //取消按键响应函数,直接退出对话框不做任何处理 
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog, int which) { 
									}
								}).show();  //显示对话框
	} 
}