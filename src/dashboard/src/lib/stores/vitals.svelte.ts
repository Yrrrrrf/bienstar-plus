import { type VitalSign, type ApiReading } from '$lib/types';

interface ApiResponse {
    current: ApiReading;
    history: ApiReading[];
}

class VitalsStore {
    // Initial state
    vitals = $state<VitalSign[]>([
        { id: 'hr', name: 'Ritmo Cardíaco', value: 0, unit: 'bpm', status: 'normal', min: 60, max: 100 },
        { id: 'spo2', name: 'Oxigenación', value: 0, unit: '%', status: 'normal', min: 95, max: 100 },
        { id: 'temp', name: 'Temperatura', value: 0, unit: '°C', status: 'normal', min: 36, max: 37.5 }
    ]);
    
    history = $state<ApiReading[]>([]);

    connectionStatus = $state<'connected' | 'disconnected'>('disconnected');
    lastUpdated = $state<string | null>(null);

    private pollingInterval: ReturnType<typeof setInterval> | null = null;
    private failCount = 0;

    constructor() {
        // No simulation in constructor
    }

    async fetchVitals() {
        try {
            const response = await fetch('/api/vitals');
            if (!response.ok) throw new Error('Fetch failed');
            
            const data: ApiResponse = await response.json();
            this.updateVitals(data.current);
            this.history = data.history;
            
            this.connectionStatus = 'connected';
            this.failCount = 0;
            this.lastUpdated = new Date().toISOString();

        } catch (error) {
            console.error('Error fetching vitals:', error);
            this.failCount++;
            if (this.failCount >= 3) {
                this.connectionStatus = 'disconnected';
            }
        }
    }

    private updateVitals(reading: ApiReading) {
        // Map API reading to VitalSign objects
        this.vitals = this.vitals.map(v => {
            let newValue = v.value;
            let newStatus = v.status;

            if (v.id === 'hr') {
                newValue = reading.hr;
            } else if (v.id === 'spo2') {
                newValue = reading.spo2;
            } else if (v.id === 'temp') {
                newValue = reading.temp;
            }

            // Determine status locally
            if (v.id === 'hr' && newValue > 120) newStatus = 'critical';
            else if (v.id === 'spo2' && newValue < 90) newStatus = 'warning';
            else if (newValue < v.min || newValue > v.max) newStatus = 'warning';
            else newStatus = 'normal';

            return { ...v, value: newValue, status: newStatus as any };
        });
    }

    startPolling(intervalMs: number = 2000) {
        this.stopPolling();
        this.fetchVitals(); // Initial fetch
        this.pollingInterval = setInterval(() => {
            this.fetchVitals();
        }, intervalMs);
    }

    stopPolling() {
        if (this.pollingInterval) {
            clearInterval(this.pollingInterval);
            this.pollingInterval = null;
        }
    }

    async simulateEmergency() {
         // Post a critical reading to the API to simulate external device
         try {
             await fetch('/api/vitals', {
                 method: 'POST',
                 headers: { 'Content-Type': 'application/json' },
                 body: JSON.stringify({
                     hr: 145,
                     spo2: 88,
                     temp: 38.5,
                     status: 'critical',
                     timestamp: new Date().toISOString()
                 })
             });
             this.fetchVitals(); // Update immediately
         } catch (e) {
             console.error("Simulation failed", e);
         }
    }

    stopMonitoring() {
        this.stopPolling();
        this.connectionStatus = 'disconnected';
    }
}

export const vitalsStore = new VitalsStore();