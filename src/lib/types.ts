export interface VitalSign {
  id: string;
  name: string;
  value: number;
  unit: string;
  status: 'normal' | 'warning' | 'critical';
  min: number;
  max: number;
}
