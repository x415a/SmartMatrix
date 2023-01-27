#include "define.h"
#include "microphone.h"

#ifdef AUDIO_PIN

class AudioFilter {
public:
  AudioFilter(float start, float coef): _fil(start), _coef(coef) {}

  float filter(float value) {
      _fil += (value - _fil) * _coef;
      _updateSample();
      return _fil;
  }

  float getRawState() {
    return _fil;
  }

  float getState() {
    return _amp;
  }

protected:
  float _coef = 0.0, _fil = 0.0;

  void _updateSample() {
    _amp_temp = (abs(_fil) > abs(_amp_temp)) ? _fil : _amp_temp;

    if (_sample_counter >= AUDIO_SAMPLE) {
      _amp = _amp_temp;
      _amp_temp = _fil;
      _sample_counter = 0;
    }

    _sample_counter++;
  }

private:
    float _amp = 0.0, _amp_temp = 0.0;
    uint16_t _sample_counter = 0;
};


class AudioInstantFilter: public AudioFilter {
public:
  AudioInstantFilter(float start, float growth, float coef): _growth(growth), AudioFilter(start, coef) {}

  float filter(float value) {
    _fil += (value - _fil) * ((value > _fil) ? _growth : _coef);
    _updateSample();
    return _fil;
  }
private:
  float _growth = 0;
};


AudioFilter signal_volume(512, 4e-3), common_volume(0, 25e-4);
AudioInstantFilter global_volume(0, .8, 2e-3), dynamic_vl_range(0, .7, .2);


void initMicrophone() {
  pinMode(AUDIO_PIN, INPUT);

  Timer2.setFrequency(AUDIO_FR);
  Timer2.enableISR(CHANNEL_B);
}


ISR(TIMER2_B) {
  int16_t signal = analogRead(AUDIO_PIN);
  float amp = abs(signal - signal_volume.filter(signal));
  common_volume.filter(max(AUDIO_THRESHOLD, amp * AUDIO_AMP_GAIN));
  amp = max(0, amp - common_volume.getRawState());
  global_volume.filter(amp);
  dynamic_vl_range.filter(amp);

  Serial.print(amp);
  Serial.print(',');
  Serial.print(global_volume.getRawState());
  Serial.print(',');
  Serial.print(dynamic_vl_range.getRawState());
  Serial.println();
}


float _clampAmpRange(float v) {
  return constrain(v - AUDIO_THRESHOLD, 0, AUDIO_MAX_AMP);
}


float _convertRawVolume(float v) {
  static const float range = log(AUDIO_MAX_AMP + 1);
  return log(v + 1) * AUDIO_MAX_VOLUME / range;
}


uint8_t getInstantAudioVolume() {
  float raw_volume = _clampAmpRange(common_volume.getState() + dynamic_vl_range.getState());
  return _convertRawVolume(raw_volume);
}


uint8_t getAudioVolume() {
  return _convertRawVolume(_clampAmpRange(global_volume.getState()));
}


void printAudioInfo() {
  Serial.print(global_volume.getState());
  Serial.print(",");
  Serial.print(common_volume.getState());
  Serial.print(",");
  Serial.print(dynamic_vl_range.getState());
  Serial.print(",");
  Serial.print(getAudioVolume());
  Serial.print(',');
  Serial.print(getInstantAudioVolume());

  Serial.println();
}

#endif
