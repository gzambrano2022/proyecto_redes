// Constantes de configuración
const CONFIG = {
  apiEndpoint: "/api/sensor-data",
  defaultLimit: 50,
  chartOptions: {
    responsive: true,
    interaction: {
      mode: "index",
      intersect: false,
    },
    scales: {
      y: {
        title: { display: true, text: "Temperatura (°C)" },
        position: "left",
      },
      y1: {
        title: { display: true, text: "Presión (hPa)" },
        position: "right",
        grid: { drawOnChartArea: false },
      },
      y2: {
        title: { display: true, text: "Humedad (%)" },
        position: "right",
        grid: { drawOnChartArea: false },
        min: 0,
        max: 100,
      },
    },
  },
};

// Variables globales
let chartInstance = null;

// Función principal de inicialización
document.addEventListener("DOMContentLoaded", async () => {
  try {
    // Cargar datos iniciales
    const initialData = await loadInitialData();

    // Inicializar componentes
    initializeChart(initialData);
    setupEventListeners();

    // Mostrar datos iniciales
    updateUI(initialData);

    console.log("Aplicación inicializada correctamente");
  } catch (error) {
    console.error("Error al inicializar la aplicación:", error);
    showError("Error al cargar los datos iniciales");
  }
});

// Cargar datos iniciales
async function loadInitialData() {
  try {
    const response = await fetch(
      `${CONFIG.apiEndpoint}?limit=${CONFIG.defaultLimit}`
    );
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    return await response.json();
  } catch (error) {
    console.error("Error al cargar datos iniciales:", error);
    throw error;
  }
}

// Inicializar gráfico
function initializeChart(data) {
  const ctx = document.getElementById("sensorChart");
  if (!ctx) {
    throw new Error("Elemento del gráfico no encontrado");
  }

  chartInstance = new Chart(ctx.getContext("2d"), {
    type: "line",
    data: formatChartData(data),
    options: CONFIG.chartOptions,
  });
}

// Formatear datos para el gráfico
function formatChartData(data) {
  return {
    labels: data.map((item) => new Date(item.timestamp).toLocaleString()),
    datasets: [
      {
        label: "Temperatura (°C)",
        data: data.map((item) => item.temperatura),
        borderColor: "rgba(255, 99, 132, 1)",
        backgroundColor: "rgba(255, 99, 132, 0.2)",
        yAxisID: "y",
        tension: 0.1,
      },
      {
        label: "Presión (hPa)",
        data: data.map((item) => item.presion),
        borderColor: "rgba(54, 162, 235, 1)",
        backgroundColor: "rgba(54, 162, 235, 0.2)",
        yAxisID: "y1",
        tension: 0.1,
      },
      {
        label: "Humedad (%)",
        data: data.map((item) => item.humedad),
        borderColor: "rgba(75, 192, 192, 1)",
        backgroundColor: "rgba(75, 192, 192, 0.2)",
        yAxisID: "y2",
        tension: 0.1,
      },
    ],
  };
}

// Configurar event listeners
function setupEventListeners() {
  const form = document.getElementById("filter-form");
  const sensorSelect = document.getElementById("sensor-select");

  if (form && sensorSelect) {
    form.addEventListener("submit", handleFilterSubmit);
    sensorSelect.addEventListener("change", handleFilterChange);
  }
}

// Manejar envío de formulario
async function handleFilterSubmit(e) {
  e.preventDefault();
  await applyFilters();
}

// Manejar cambio en el select
async function handleFilterChange() {
  await applyFilters();
}

// Aplicar filtros
async function applyFilters() {
  try {
    showLoading(true);

    const sensorId = document.getElementById("sensor-select").value;
    const data = await fetchData(sensorId);

    updateUI(data);
  } catch (error) {
    console.error("Error al aplicar filtros:", error);
    showError("Error al aplicar filtros");
  } finally {
    showLoading(false);
  }
}

// Obtener datos filtrados
async function fetchData(sensorId) {
  try {
    const url = `${CONFIG.apiEndpoint}?limit=${CONFIG.defaultLimit}${
      sensorId ? `&sensor_id=${sensorId}` : ""
    }`;
    const response = await fetch(url);

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    return await response.json();
  } catch (error) {
    console.error("Error al obtener datos:", error);
    throw error;
  }
}

// Actualizar interfaz de usuario
function updateUI(data) {
  updateTable(data);
  updateChart(data);
}

// Actualizar tabla
function updateTable(data) {
  const tbody = document.getElementById("sensor-data");
  tbody.innerHTML = data
    .map(
      (item) => `
        <tr>
            <td>${item.id}</td>
            <td>${item.sensor_id}</td>
            <td data-timestamp="${item.timestamp}">${formatDateTime(
        item.timestamp
      )}</td>
            <td>${item.temperatura.toFixed(1)}</td>
            <td>${item.presion.toFixed(2)}</td>
            <td>${item.humedad.toFixed(1)}</td>
        </tr>
    `
    )
    .join("");

  // Vuelve a aplicar el ordenamiento después de actualizar
  sortAndDisplayData();
}

// Actualizar gráfico
function updateChart(data) {
  if (chartInstance) {
    chartInstance.data = formatChartData(data);
    chartInstance.update();
  }
}

// Mostrar/ocultar estado de carga
function showLoading(show) {
  const form = document.getElementById("filter-form");
  if (form) {
    form.classList.toggle("loading", show);
  }
}

// Mostrar mensaje de error
function showError(message) {
  const errorElement = document.getElementById("error-message");
  if (errorElement) {
    errorElement.textContent = message;
    errorElement.style.display = "block";
    setTimeout(() => {
      errorElement.style.display = "none";
    }, 5000);
  }
}

// Función para manejar la visibilidad del gráfico
function toggleChartVisibility(show) {
  const chartContainer = document.getElementById("chart-container");
  const noChartMessage = document.getElementById("no-chart-message");

  if (show) {
    chartContainer.style.display = "block";
    noChartMessage.style.display = "none";
  } else {
    chartContainer.style.display = "none";
    noChartMessage.style.display = "block";
  }
}

// Modificación en la función applyFilters
async function applyFilters() {
  try {
    showLoading(true);

    const sensorId = document.getElementById("sensor-select").value;
    const data = await fetchData(sensorId);

    // Mostrar/ocultar gráfico según si hay sensor seleccionado
    toggleChartVisibility(!!sensorId);

    updateUI(data);
  } catch (error) {
    console.error("Error al aplicar filtros:", error);
    showError("Error al aplicar filtros");
  } finally {
    showLoading(false);
  }
}

// Inicialización - ocultar gráfico al inicio
document.addEventListener("DOMContentLoaded", function () {
  toggleChartVisibility(false); // Oculta el gráfico inicialmente
  // Resto de tu código de inicialización...
});
// Variables para el estado de ordenamiento
let currentSort = {
  column: "timestamp",
  direction: "desc", // Por defecto ordenado por fecha más reciente primero
};

// Función para inicializar los event listeners de ordenamiento
function initSorting() {
  document.querySelectorAll(".sortable").forEach((header) => {
    header.addEventListener("click", () => {
      const column = header.dataset.column;

      // Determinar nueva dirección
      if (currentSort.column === column) {
        currentSort.direction =
          currentSort.direction === "asc" ? "desc" : "asc";
      } else {
        currentSort.column = column;
        currentSort.direction = "asc"; // Por defecto ascendente al cambiar de columna
      }

      // Actualizar UI y ordenar
      updateSortIcons();
      sortAndDisplayData();
    });
  });
}

// Función para actualizar los íconos de ordenamiento
function updateSortIcons() {
  document.querySelectorAll(".sortable").forEach((header) => {
    header.classList.remove("asc", "desc");
    if (header.dataset.column === currentSort.column) {
      header.classList.add(currentSort.direction);
    }
  });
}

// Función para ordenar y mostrar los datos
function sortAndDisplayData() {
  const data = Array.from(document.querySelectorAll("#sensor-data tr")).map(
    (row) => ({
      element: row,
      id: parseInt(row.cells[0].textContent),
      sensor_id: parseInt(row.cells[1].textContent),
      timestamp: row.cells[2].getAttribute("data-timestamp"), // Usamos el valor ISO original
      temperatura: parseFloat(row.cells[3].textContent),
      presion: parseFloat(row.cells[4].textContent),
      humedad: parseFloat(row.cells[5].textContent),
    })
  );

  // Ordenar los datos
  data.sort((a, b) => {
    let valueA = a[currentSort.column];
    let valueB = b[currentSort.column];

    // Manejar fechas y números
    if (valueA instanceof Date) {
      return currentSort.direction === "asc"
        ? valueA - valueB
        : valueB - valueA;
    }

    return currentSort.direction === "asc"
      ? valueA > valueB
        ? 1
        : -1
      : valueB > valueA
      ? 1
      : -1;
  });

  // Reordenar las filas en la tabla
  const tbody = document.getElementById("sensor-data");
  data.forEach((item) => {
    tbody.appendChild(item.element);
  });
}

// Inicializar al cargar la página
document.addEventListener("DOMContentLoaded", function () {
  initSorting();
  updateSortIcons(); // Mostrar el orden inicial por fecha descendente
});
function formatDateTime(isoString) {
  if (!isoString) return "";

  try {
    const date = new Date(isoString);
    if (isNaN(date.getTime())) return isoString; // Si no es una fecha válida

    const timeStr = date.toLocaleTimeString("es-CL", {
      hour12: false,
      hour: "2-digit",
      minute: "2-digit",
      second: "2-digit",
    });

    const dateStr = date
      .toLocaleDateString("es-CL", {
        day: "2-digit",
        month: "2-digit",
        year: "numeric",
      })
      .replace(/\//g, "-");

    return `${timeStr} ${dateStr}`;
  } catch (e) {
    console.error("Error formateando fecha:", e);
    return isoString;
  }
}
