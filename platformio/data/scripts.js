const btn = document.getElementById('toggleBtn');
btn.addEventListener('click', () => {
    fetch('/toggle')
        .then(response => {
            if (response.ok) {
                btn.classList.toggle('active');
            }
        });
});