<script lang="ts">
    import { getLocale, setLocale, locales } from '$lib/paraglide/runtime';
    // Importamos el tipo para complacer a TypeScript si es necesario, 
    // o simplemente usamos un cast en la función.
    import { onMount } from 'svelte';

    let theme = $state('light');

    onMount(() => {
        theme = localStorage.getItem('theme') || 'light';
        document.documentElement.setAttribute('data-theme', theme);
    });

    function switchLanguage(newLang: string) {
        setLocale(newLang as "en" | "es" | "de"); 
    }
    
    function getLangName(code: string) {
        const names: Record<string, string> = {
            en: 'English',
            es: 'Español',
            de: 'Deutsch'
        };
        return names[code] || code.toUpperCase();
    }
</script>

<header class="navbar bg-base-100 shadow-md rounded-box mb-8">
    <div class="flex-1">
        <a href="/" class="btn btn-ghost text-xl">Bienestar+</a>
    </div>
    <div class="flex-none flex items-center gap-2">
        <a href="/medic" class="btn btn-ghost btn-sm">
            <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5 mr-1" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5H7a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2V7a2 2 0 00-2-2h-2M9 5a2 2 0 002 2h2a2 2 0 002-2M9 5a2 2 0 012-2h2a2 2 0 012 2" />
            </svg>
            Medic View
        </a>

        <!-- Language Dropdown -->
        <div class="dropdown dropdown-end">
            <div role="button" tabindex="0" class="btn btn-ghost">
                {getLocale().toUpperCase()}
                <svg width="12px" height="12px" class="h-2 w-2 fill-current opacity-60 inline-block" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 2048 2048"><path d="M1799 349l242 241-1017 1017L7 590l242-241 775 775 775-775z"></path></svg>
            </div>
            <ul tabindex="0" class="dropdown-content z-[1] p-2 shadow-2xl bg-base-300 rounded-box w-32">
                {#each locales as lang}
                    <li>
                        <button onclick={() => switchLanguage(lang)} class="btn btn-sm btn-ghost justify-start">
                            {getLangName(lang)}
                        </button>
                    </li>
                {/each}
            </ul>
        </div>
    </div>
</header>