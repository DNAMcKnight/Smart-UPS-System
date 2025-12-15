const complete_shutdown = document.getElementById('complete_shutdown');
const toggle_ups = document.getElementById('toggle_ups');
const pc_shutdown = document.getElementById('pc_shutdown');
const toggleBtn3 = document.getElementById('toggleBtn3');

document.addEventListener('DOMContentLoaded', async () => {
    const response = await fetch('/settings')
    const data = await response.json();
    if (data.PC_ACTIVE) {
        toggle_ups.disabled = true;
    } else {
        toggle_ups.disabled = false;
    }
});
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
        setTimeout(() => { complete_shutdown.checked = false; }, 2000);
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
        setTimeout(() => { pc_shutdown.checked = false; }, 2000);
    }
});



toggleBtn3.disabled = true; // Currently we have no use for this button