package com.anc.app;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;
import com.google.android.material.button.MaterialButton;
import com.google.android.material.materialswitch.MaterialSwitch;

public class MainActivity extends AppCompatActivity {

    static { System.loadLibrary("ancapp"); }

    // Métodos Nativos
    public native void startAudioEngine();
    public native void stopAudioEngine();
    public native void setPhaseInverted(boolean inverted);
    public native void setGain(float gain);
    public native float getCurrentAmplitude(); // Novo: Leitura de volume

    private boolean isRunning = false;
    private MaterialButton btnToggle;
    private View viewWaveLine; // A linha visual

    // Handler para animação simples
    private Handler handler = new Handler(Looper.getMainLooper());
    private Runnable updateVisualizer = new Runnable() {
        @Override
        public void run() {
            if (isRunning) {
                // Pega amplitude (0.0 a 1.0)
                float amp = getCurrentAmplitude();

                // Anima a escala vertical da linha (efeito pulsação)
                // 1.0f é o tamanho base, + (amp * 40) faz ela crescer com o som
                float targetScale = 1.0f + (amp * 40.0f);
                viewWaveLine.setScaleY(targetScale);

                // Roda novamente em 50ms (20fps)
                handler.postDelayed(this, 50);
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnToggle = findViewById(R.id.btnToggle);
        viewWaveLine = findViewById(R.id.viewWaveLine); // Referência da linha
        MaterialSwitch switchPhase = findViewById(R.id.switchPhase);
        SeekBar seekBarGain = findViewById(R.id.seekBarGain);
        TextView tvGainVal = findViewById(R.id.tvGainVal);

        btnToggle.setOnClickListener(v -> toggleAudio());

        switchPhase.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setPhaseInverted(isChecked);
        });

        seekBarGain.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                float gainValue = progress / 100.0f;
                tvGainVal.setText(String.format("%.1f", gainValue));
                setGain(gainValue);
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
    }

    private void toggleAudio() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO}, 1);
            return;
        }

        if (isRunning) {
            stopAudioEngine();
            handler.removeCallbacks(updateVisualizer); // Para animação
            viewWaveLine.setScaleY(1.0f); // Reseta visual

            btnToggle.setText("ATIVAR MICROFONE");
            btnToggle.setBackgroundColor(Color.parseColor("#0891b2"));
            isRunning = false;
        } else {
            startAudioEngine();
            handler.post(updateVisualizer); // Inicia animação

            btnToggle.setText("PARAR");
            btnToggle.setBackgroundColor(Color.parseColor("#dc2626"));
            isRunning = true;
        }
    }
}