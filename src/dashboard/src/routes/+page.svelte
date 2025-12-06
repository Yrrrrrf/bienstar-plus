<script lang="ts">
	import { onMount } from 'svelte';
	import * as m from '$lib/paraglide/messages';
	import { vitalsStore } from '$lib/stores/vitals.svelte';
	import VitalCard from '$lib/components/VitalCard.svelte';
	import AlertBanner from '$lib/components/AlertBanner.svelte';
	import Header from '$lib/components/Header.svelte';

	const { vitals } = vitalsStore;

	const hasCriticalAlert = $derived(vitals.some(v => v.status === 'critical'));

</script>

<div class="prose prose-xl container mx-auto p-4">

	<Header />

	<main>
		{#if hasCriticalAlert}
			<div class="mb-4">
				<AlertBanner />
			</div>
		{/if}

		<div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
			{#each vitals as vital (vital.id)}
				<VitalCard {vital} />
			{/each}
		</div>

		<div class="mt-8 flex justify-center gap-4">
			<button class="btn btn-warning" onclick={() => vitalsStore.simulateEmergency()}>{m.simulate_emergency()}</button>
			<button class="btn btn-accent" onclick={() => vitalsStore.stopMonitoring()}>{m.stop_monitoring()}</button>
		</div>
	</main>
</div>
