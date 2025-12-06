export interface VitalSign {
  id: string;
  name: string;
  value: number;
  unit: string;
  status: 'normal' | 'warning' | 'critical';
  min: number;
  max: number;
  history?: number[];
}

export interface ApiReading {
    hr: number;
    spo2: number;
    temp: number;
    status: string;
    timestamp: string;
}
