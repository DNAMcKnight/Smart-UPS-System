const toggle_ups = document.getElementById('toggle_ups');
toggle_ups.addEventListener('click', () => {
    fetch('/toggle')
        .then(response => {
            if (response.ok) {
                toggle_ups.classList.toggle('active');
            }
        });
});


const complete_shutdown = document.getElementById('complete_shutdown');
complete_shutdown.addEventListener('click', () => {
    console.log("Clicked");
    fetch('/complete_shutdown')
        .then(response => {
            if (response.ok) {
                complete_shutdown.classList.toggle('active');
            }
        });
});

const shutdown = document.getElementById('shutdown');
shutdown.addEventListener('click', () => {
    fetch('/shutdown')
        .then(response => {
            if (response.ok) {
                complete_shutdown.classList.toggle('active');
            }
        });
});