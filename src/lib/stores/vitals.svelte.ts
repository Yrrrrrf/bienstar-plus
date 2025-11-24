import { type VitalSign } from '$lib/types';

class VitalsStore {
  vitals = $state<VitalSign[]>([
    { id: 'hr', name: 'Ritmo Cardíaco', value: 80, unit: 'bpm', status: 'normal', min: 60, max: 100 },
    { id: 'spo2', name: 'Oxigenación', value: 98, unit: '%', status: 'normal', min: 95, max: 100 },
    { id: 'temp', name: 'Temperatura', value: 36.5, unit: '°C', status: 'normal', min: 36, max: 37.5 }
  ]);

  constructor() {
    this.simulateData();
  }

  private simulateData() {
    setInterval(() => {
      this.vitals = this.vitals.map(vital => {
        const change = (Math.random() - 0.5) * (vital.id === 'temp' ? 0.2 : 5);
        let newValue = vital.value + change;

        // Clamp values to a reasonable range
        if (vital.id === 'hr') newValue = Math.max(50, Math.min(150, newValue));
        if (vital.id === 'spo2') newValue = Math.max(85, Math.min(100, newValue));
        if (vital.id === 'temp') newValue = Math.max(35, Math.min(41, newValue));
        
        vital.value = parseFloat(newValue.toFixed(1));

        // Update status
        if (vital.id === 'hr' && vital.value > 120) {
          vital.status = 'critical';
        } else if (vital.id === 'spo2' && vital.value < 90) {
          vital.status = 'warning';
        } else if (vital.value < vital.min || vital.value > vital.max) {
          vital.status = 'warning';
        } else {
          vital.status = 'normal';
        }
        
        return vital;
      });
    }, 2000);
  }

  simulateEmergency() {
    const hr = this.vitals.find(v => v.id === 'hr');
    if (hr) {
      hr.value = 140;
    }
  }

  stopMonitoring() {
    // In a real app, this would clear intervals/disconnect.
    // For now, we can reset to a normal state.
    this.vitals = [
        { id: 'hr', name: 'Ritmo Cardíaco', value: 80, unit: 'bpm', status: 'normal', min: 60, max: 100 },
        { id: 'spo2', name: 'Oxigenación', value: 98, unit: '%', status: 'normal', min: 95, max: 100 },
        { id: 'temp', name: 'Temperatura', value: 36.5, unit: '°C', status: 'normal', min: 36, max: 37.5 }
      ];
  }
}

export const vitalsStore = new VitalsStore();
