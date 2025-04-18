// Define an interface for a User object
// server.ts
import { createServer, IncomingMessage, ServerResponse } from 'http';
import { URL } from 'url';
import { promises as fs } from 'fs';

interface Task {
  id: number;
  title: string;
  description?: string;
}

const DB_FILE = 'tasks.json';
let tasks: Task[] = [];

// Load initial data
async function loadTasks() {
  try {
    const data = await fs.readFile(DB_FILE, 'utf-8');
    tasks = JSON.parse(data);
  } catch (error) {
    tasks = [];
  }
}

// Save tasks to file
async function saveTasks() {
  await fs.writeFile(DB_FILE, JSON.stringify(tasks, null, 2));
}

const server = createServer(async (req: IncomingMessage, res: ServerResponse) => {
  const parsedUrl = new URL(req.url || '', `http://${req.headers.host}`);
  
  // Set CORS headers
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Content-Type', 'application/json');

  try {
    // GET all tasks
    if (req.method === 'GET' && parsedUrl.pathname === '/tasks') {
      res.writeHead(200);
      res.end(JSON.stringify(tasks));
    }
    // POST new task
    else if (req.method === 'POST' && parsedUrl.pathname === '/tasks') {
      let body = '';
      req.on('data', chunk => body += chunk);
      
      req.on('end', async () => {
        const newTask: Task = {
          id: tasks.length + 1,
          ...JSON.parse(body)
        };
        
        tasks.push(newTask);
        await saveTasks();
        res.writeHead(201);
        res.end(JSON.stringify(newTask));
      });
    }
    else {
      res.writeHead(404);
      res.end(JSON.stringify({ error: 'Not Found' }));
    }
  } catch (error) {
    res.writeHead(500);
    res.end(JSON.stringify({ error: 'Server Error' }));
  }
});

// Initialize and start server
loadTasks().then(() => {
  server.listen(3000, () => {
    console.log('Server running on http://localhost:3000');
  });
});