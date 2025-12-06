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

</script>

<div class="card bg-base-100 shadow-xl border-2 {statusClasses}">
	<div class="card-body">
		<div class="stat">
			<div class="stat-title">{name}</div>
			<div class="stat-value">{vital.value.toFixed(vital.id === 'temp' ? 1 : 0)} <span class="text-2xl">{vital.unit}</span></div>
			<div class="stat-desc">Min: {vital.min} / Max: {vital.max}</div>
		</div>
	</div>
</div>