package com.example.tofdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.serialworker.serialportsdk.serial.SerialManager;
import com.serialworker.serialportsdk.serial.command.recv.RecvTOFDistance;
import com.serialworker.serialportsdk.serial.command.send.SendTOFDistance;
import com.serialworker.serialportsdk.serial.core.Callback;

public class MainActivity extends AppCompatActivity {



    private static String TOF_SERIAL = "/dev/ttyS4";
    private static int TOF_BAUDRATE = 9600;
    private Toast mToast;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SerialManager.getInstance().initDevice(TOF_SERIAL,TOF_BAUDRATE);
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        SerialManager.getInstance().release();
    }

    public void open_tof(View view) {
        SendTOFDistance command = new SendTOFDistance();

        SerialManager.getInstance().openTOF(command, new Callback<RecvTOFDistance>() {
            @Override
            public void onSuccess(RecvTOFDistance recvTOFDistance) {
                showToast("距离 = "+recvTOFDistance.getDistance());
            }

            @Override
            public void onFailure(Throwable tr) {
                Log.w("onError", tr);
                showToast(tr.getMessage());
            }
        });
    }
    private void showToast(String s) {
        if (mToast != null) {
            mToast.cancel();
        }
        mToast = Toast.makeText(this, s, Toast.LENGTH_LONG);
        mToast.show();
    }
}