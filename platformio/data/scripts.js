const complete_shutdown = document.getElementById('complete_shutdown');
const toggle_ups = document.getElementById('toggle_ups');
const pc_shutdown = document.getElementById('pc_shutdown');
const toggleBtn3 = document.getElementById('toggleBtn3');

document.addEventListener('DOMContentLoaded', () => { });
toggle_ups.addEventListener('click', async () => {
    toggle_ups.disabled = true;
    try {
        const response = await fetch('/toggle')
        if (response.ok) {
            toggle_ups.classList.toggle('active');
        }
    } catch (err) {
        console.error('Error:', err);
    }
    finally {
        toggle_ups.disabled = false;
    }

});



complete_shutdown.addEventListener('click', async () => {
    complete_shutdown.disabled = true;
    try {
        const response = await fetch('/complete_shutdown')
        if (response.ok) {
            complete_shutdown.classList.toggle('active');
        }
    } catch (err) {
        console.error('Error:', err);
    } finally {
        complete_shutdown.disabled = false;
    }
});


pc_shutdown.addEventListener('click', async () => {
    pc_shutdown.disabled = true;
    try {
        const response = await fetch('/pc_shutdown')
        if (response.ok) {
            pc_shutdown.classList.toggle('active');
        }
    } catch (err) {
        console.error('Error:', err);
    } finally {
        pc_shutdown.disabled = false;
    }
});



toggleBtn3.disabled = true; // Currently we have no use for this button