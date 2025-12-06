<script lang="ts">
	import type { VitalSign } from '$lib/types';
	import * as m from '$lib/paraglide/messages';

	let { vital } = $props<{ vital: VitalSign }>();

    // CORRECCIÓN: Usar $derived.by para ejecutar la lógica de la función
	const statusClasses = $derived.by(() => {
		switch (vital.status) {
			case 'critical':
				return 'border-red-500 bg-red-50';
			case 'warning':
				return 'border-yellow-500 bg-yellow-50';
			default:
				return 'border-green-500 bg-green-50';
		}
	});

    // CORRECCIÓN: Usar $derived.by aquí también
	const name = $derived.by(() => {
		switch (vital.id) {
			case 'hr':
				return m.heart_rate();
			case 'spo2':
				return m.oxygenation();
			case 'temp':
				return m.temperature();
			default:
				return vital.name;
		}
	});

    const historyPoints = $derived.by(() => {
        const h = vital.history || [];
        if (h.length < 2) return '';
        
        const data = h.slice(-20); // Last 20 points
        if (data.length === 0) return '';

        const min = Math.min(...data);
        const max = Math.max(...data);
        // Ensure some range to avoid division by zero
        const range = (max - min) || 1; 
        
        const height = 50; 
        const width = 200; // Internal coordinate system width
        const step = width / (data.length - 1);

        return data.map((val: number, i: number) => {
            const x = i * step;
            // Normalize value to 0-1, then map to height (leaving 5px padding top/bottom)
            const normalized = (val - min) / range;
            const y = height - 5 - (normalized * (height - 10)); 
            return `${x},${y}`;
        }).join(' ');
    });

    const strokeClass = $derived.by(() => {
        switch (vital.status) {
            case 'critical': return 'stroke-red-600';
            case 'warning': return 'stroke-yellow-600';
            default: return 'stroke-green-600';
        }
    });

</script>

<div class="card bg-base-100 shadow-xl border-2 {statusClasses} overflow-hidden relative">
	<div class="card-body relative z-10">
		<div class="stat p-0">
			<div class="stat-title">{name}</div>
			<div class="stat-value">{vital.value.toFixed(vital.id === 'temp' ? 1 : 0)} <span class="text-2xl">{vital.unit}</span></div>
			<div class="stat-desc">Min: {vital.min} / Max: {vital.max}</div>
		</div>
	</div>
    
    <!-- Sparkline Background -->
    <div class="absolute bottom-0 left-0 right-0 h-16 opacity-50 pointer-events-none">
        {#if historyPoints}
            <svg viewBox="0 0 200 50" class="w-full h-full" preserveAspectRatio="none">
                <polyline 
                    points={historyPoints} 
                    fill="none" 
                    stroke-width="3" 
                    class="{strokeClass}"
                    stroke-linecap="round"
                    stroke-linejoin="round"
                />
            </svg>
        {/if}
    </div>
</div>