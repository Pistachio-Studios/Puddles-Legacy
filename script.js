let lastScrollTop = 0;

window.addEventListener("scroll", function(event) {
    let currentScroll = window.pageYOffset || document.documentElement.scrollTop;
    let header = document.getElementById("stickyMenu");

    if (event.deltaY > 0) {
        // Desplazamiento hacia abajo
        if (!header.classList.contains("shrink") && currentScroll > 100) {
            header.classList.add("shrink");
        }
    } else {
        // Desplazamiento hacia arriba
        header.classList.remove("shrink");
    }
});
