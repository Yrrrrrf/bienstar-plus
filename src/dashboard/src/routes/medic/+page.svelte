<script lang="ts">
    import { vitalsStore } from '$lib/stores/vitals.svelte';
    import type { ApiReading } from '$lib/types';

    // Sort history by timestamp descending (newest first)
    let sortedHistory = $derived([...vitalsStore.history].sort((a, b) => 
        new Date(b.timestamp).getTime() - new Date(a.timestamp).getTime()
    ));

    // Filter for alerts (anything not 'normal')
    let alerts = $derived(sortedHistory.filter(h => h.status !== 'normal'));

    function exportCSV() {
        const headers = ['Timestamp', 'Heart Rate (bpm)', 'SpO2 (%)', 'Temperature (°C)', 'Status'];
        const rows = sortedHistory.map(h => [
            h.timestamp,
            h.hr,
            h.spo2,
            h.temp,
            h.status
        ]);
        
        const csvContent = [
            headers.join(','),
            ...rows.map(r => r.join(','))
        ].join('\n');

        const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
        const url = URL.createObjectURL(blob);
        const link = document.createElement('a');
        link.setAttribute('href', url);
        link.setAttribute('download', 'medical_history.csv');
        link.style.visibility = 'hidden';
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);
    }

    function formatTime(isoString: string) {
        try {
            return new Date(isoString).toLocaleTimeString();
        } catch (e) {
            return isoString;
        }
    }
</script>

<div class="container mx-auto p-4">
    <div class="flex justify-between items-center mb-6">
        <h1 class="text-2xl font-bold">Medic View - Patient History</h1>
        <button class="btn btn-primary btn-sm" onclick={exportCSV}>
            <svg xmlns="http://www.w3.org/2000/svg" class="h-4 w-4 mr-2" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 16v1a3 3 0 003 3h10a3 3 0 003-3v-1m-4-4l-4 4m0 0l-4-4m4 4V4" />
            </svg>
            Export CSV
        </button>
    </div>

    <div class="grid grid-cols-1 md:grid-cols-3 gap-6">
        <!-- Timeline Column -->
        <div class="md:col-span-1">
            <h2 class="text-xl font-semibold mb-4">Alert Timeline</h2>
            {#if alerts.length === 0}
                <div class="alert alert-success">
                    <span>No alerts recorded. Patient stable.</span>
                </div>
            {:else}
                <ul class="steps steps-vertical w-full">
                    {#each alerts.slice(0, 10) as alert}
                        <li class="step" class:step-error={alert.status === 'critical'} class:step-warning={alert.status === 'warning'}>
                            <div class="text-left w-full pl-2">
                                <div class="font-bold text-sm">{formatTime(alert.timestamp)}</div>
                                <div class="text-xs opacity-70">
                                    HR: {alert.hr} | SpO2: {alert.spo2}% | T: {alert.temp}°C
                                </div>
                                <div class="badge badge-sm mt-1" class:badge-error={alert.status === 'critical'} class:badge-warning={alert.status === 'warning'}>
                                    {alert.status.toUpperCase()}
                                </div>
                            </div>
                        </li>
                    {/each}
                </ul>
            {/if}
        </div>

        <!-- Data Table Column -->
        <div class="md:col-span-2">
            <h2 class="text-xl font-semibold mb-4">Recent Readings (Last 50)</h2>
            <div class="overflow-x-auto bg-base-100 rounded-box shadow">
                <table class="table table-xs table-zebra">
                    <thead>
                        <tr>
                            <th>Time</th>
                            <th>HR (bpm)</th>
                            <th>SpO2 (%)</th>
                            <th>Temp (°C)</th>
                            <th>Status</th>
                        </tr>
                    </thead>
                    <tbody>
                        {#each sortedHistory.slice(0, 50) as reading}
                            <tr class:bg-error={reading.status === 'critical'} class:bg-warning={reading.status === 'warning'}>
                                <td>{formatTime(reading.timestamp)}</td>
                                <td class:font-bold={reading.status !== 'normal'}>{reading.hr}</td>
                                <td class:font-bold={reading.status !== 'normal'}>{reading.spo2}</td>
                                <td class:font-bold={reading.status !== 'normal'}>{reading.temp}</td>
                                <td>
                                    <span class="badge badge-xs" 
                                        class:badge-error={reading.status === 'critical'} 
                                        class:badge-warning={reading.status === 'warning'}
                                        class:badge-success={reading.status === 'normal'}>
                                        {reading.status}
                                    </span>
                                </td>
                            </tr>
                        {/each}
                        {#if sortedHistory.length === 0}
                            <tr>
                                <td colspan="5" class="text-center py-4">No data available</td>
                            </tr>
                        {/if}
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</div>
