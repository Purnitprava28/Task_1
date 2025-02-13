document.addEventListener("DOMContentLoaded", function() {
    let quantity = 1;
    const quantityDisplay = document.getElementById("quantity");
    const decreaseBtn = document.querySelector(".qty-btn:first-child");
    const increaseBtn = document.querySelector(".qty-btn:last-child");
    const thumbnails = document.querySelectorAll(".thumbnail");
    const mainImage = document.querySelector(".main-image");

    increaseBtn.addEventListener("click", function() {
        quantity++;
        quantityDisplay.textContent = quantity;
    });

    decreaseBtn.addEventListener("click", function() {
        if (quantity > 1) {
            quantity--;
            quantityDisplay.textContent = quantity;
        }
    });

    thumbnails.forEach(thumbnail => {
        thumbnail.addEventListener("click", function() {
            mainImage.src = this.src;
        });
    });
});
